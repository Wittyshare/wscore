/**
 *
 *       @file  WsMonitor.cpp
 *
 *      @brief  Monitors the FileSystem for any modification
 *
 *    @version  1.0
 *       @date  07/22/11 13:37:17
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsFsTreeUpdater.h"
#include "WsMonitor.h"
#include <Include/WsGlobalConfig.h>

WsMonitor::WsMonitor(WsFsTreeUpdater* u, vector<path> v, unsigned int delay):
  m_updater(u),
  m_paths(v),
  m_updated(false),
  m_delay(delay)
{
}

WsMonitor::~WsMonitor()
{
  LOG(DEBUG) << "WsMonitor::~WsMonitor() : Deleting this";
  if (FAMCancelMonitor(m_fc, m_frp) < 0)
    LOG(ERROR) << "WsMonitor::~WsMonitor() : Error while stopping Fam monitor ";
  else
    LOG(INFO) << "WsMonitor::~WsMonitor() : Fam stopped successfully";
  if (FAMCancelMonitor(m_fc2, m_frp2) < 0)
    LOG(ERROR) << "WsMonitor::~WsMonitor() : Error while stopping Fam monitor ";
  else
    LOG(INFO) << "WsMonitor::~WsMonitor() : Fam stopped successfully";
  delete m_frp;
  m_frp = 0;
  delete m_fc;
  m_fc = 0;
  delete m_frp2;
  m_frp2 = 0;
  delete m_fc2;
  m_fc2 = 0;
}

const char* WsMonitor::eventName(const int& code)
{
  static const char* famevent[] = {
    "",
    "FAMChanged",
    "FAMDeleted",
    "FAMStartExecuting",
    "FAMStopExecuting",
    "FAMCreated",
    "FAMMoved",
    "FAMAcknowledge",
    "FAMExists",
    "FAMEndExist"
  };
  static char unknown_event[10];
  /* Unknown code */
  if (code < FAMChanged || code > FAMEndExist) {
    LOG(ERROR) << "WsMonitor::WsMonitor() : Unknown event code returned by Gamin " << code << endl;
    return "unknown";
  }
  /* Return code */
  return famevent[code];
}

boost::thread* WsMonitor::start()
{
  /* Launch the threads that will monitor the files and the thread that will check the m_updater var */
  boost::thread* updater = new boost::thread(boost::bind(&WsMonitor::startCallBack, this));
  boost::thread* updaterConf = new boost::thread(boost::bind(&WsMonitor::startCallBackConfOnly, this));
  boost::thread* checker = new boost::thread(boost::bind(&WsMonitor::startChecker, this));
  return updater;
}

int WsMonitor::startChecker()
{
  LOG(INFO) << "WsMonitor::startChecker() : Will update FsTree every " << m_delay << "s if changes occurs";
  while (1) {
    LOG(DEBUG) << "WsMonitor::startChecker() : Checking updated status : " << m_updated;
    /* If changed, update the tree */
    if (m_updated) {
      LOG(INFO) << "WsMonitor::startChecker() : Updating FsTree";
      m_updated = false;
      m_updater->update();
    }
    sleep(m_delay);
  }
}

int WsMonitor::startCallBack()
{
  int i, nmon, rc, fam_fd;
  FAMEvent fe;
  fd_set readfds;
  string config;
  /* Allocate storage for requests */
  m_frp = (FAMRequest*)malloc(m_paths.size() * sizeof * m_frp);
  if (!m_frp) {
    LOG(ERROR) << "WsMonitor::startCallBack() : Malloc failed for Fam";
    return FAILURE;
  }
  m_fc = (FAMConnection*)malloc(sizeof(m_fc));
  /* Open fam connection */
  if ((FAMOpen(m_fc)) < 0) {
    LOG(ERROR) << "WsMonitor::startCallBack() : Fam open failed";
    return FAILURE;
  }
  /* Request monitoring for each program argument */
  for (nmon = 0, i = 0; i < m_paths.size(); i++) {
    rc = FAMMonitorDirectory(m_fc, m_paths[i].string().c_str(), m_frp + i, 0);
    if (rc < 0) {
      LOG(ERROR) << "WsMonitor::startCallBack() :  FAMMonitor failed" << endl;
      continue;
    }
    nmon++;
  }
  /* If nmon == 0, nothing to monitor */
  if (!nmon) {
    LOG(ERROR) << "WsMonitor::startCallBack() : Fam nothing to monitor." << endl;
    return FAILURE;
  }
  /* Initialize select data structure */
  fam_fd = FAMCONNECTION_GETFD(m_fc);
  FD_ZERO(&readfds);
  FD_SET(fam_fd, &readfds);
  /* Loop forever. */
  while (1) {
    if (select(fam_fd + 1, &readfds,
               0, 0, 0) < 0) {
      LOG(ERROR) << "WsMonitor::startCallBack() : Select failed" << endl;
      return FAILURE;
    }
    if (FD_ISSET(fam_fd, &readfds)) {
      if (FAMNextEvent(m_fc, &fe) < 0) {
        LOG(ERROR) << "WsMonitor::startCallBack() : FAMNextEvent Failed" << endl;
        exit(1);
      }
      signal(fe.filename,	fe.code);
    }
  }
}

int WsMonitor::startCallBackConfOnly()
{
  int i, nmon, rc, fam_fd;
  FAMEvent fe;
  fd_set readfds;
  string config;
  /* Allocate storage for requests */
  m_frp2 = (FAMRequest*)malloc(m_paths.size() * sizeof * m_frp2);
  if (!m_frp2) {
    LOG(ERROR) << "WsMonitor::startCallBackConfOnly() : Malloc failed for Fam";
    return FAILURE;
  }
  m_fc2 = (FAMConnection*)malloc(sizeof(m_fc2));
  /* Open fam connection */
  if ((FAMOpen(m_fc2)) < 0) {
    LOG(ERROR) << "WsMonitor::startCallBackConfOnly() Fam open failed";
    return FAILURE;
  }
  /* Request monitoring for each program argument */
  for (nmon = 0, i = 0; i < m_paths.size(); i++) {
    config = m_paths[i].string() + GlobalConfig::PathToNodeProperties;
    ifstream fconf(config.c_str());
    if (fconf.good()) {
      rc = FAMMonitorDirectory(m_fc2, config.c_str(), m_frp2 + i, 0);
      if (rc < 0) {
        LOG(ERROR) << "WsMonitor::startCallBackConfOnly() : FAMMonitor failed" << endl;
        continue;
      }
      nmon++;
    }
    nmon++;
  }
  if (!nmon) {
    LOG(ERROR) << "WsMonitor::startCallBackConfOnly() : Fam nothing to monitor." << endl;
    return FAILURE;
  }
  /* Initialize select data structure */
  fam_fd = FAMCONNECTION_GETFD(m_fc2);
  FD_ZERO(&readfds);
  FD_SET(fam_fd, &readfds);
  /* Loop forever. */
  while (1) {
    if (select(fam_fd + 1, &readfds,
               0, 0, 0) < 0) {
      LOG(ERROR) << "WsMonitor::startCallBackConfOnly() : Select failed" << endl;
      return FAILURE;
    }
    if (FD_ISSET(fam_fd, &readfds)) {
      if (FAMNextEvent(m_fc2, &fe) < 0) {
        LOG(ERROR) << "WsMonitor::startCallBackConfOnly() : FAMNextEvent Failed" << endl;
        exit(1);
      }
      signalConf(fe.filename,	fe.code);
    }
  }
}

int WsMonitor::signal(const string& filename, const int& code)
{
  string rootpath = WsGlobalProperties::instance()->get("global", "root_path", ".");
  string temp = rootpath + GlobalConfig::PathToLockFile;
  temp = boost::filesystem::path(temp).string();
  ifstream fconf(temp.c_str());
  if (fconf.good()) {
    LOG(DEBUG) << "WsMonitor::signal() : Lock File is present. Not updating";
    return SUCCESS;
  }
  switch (code) {
  case FAMDeleted:
    LOG(DEBUG) << "WsMonitor::signal() Change occured on  " << filename << " - Code FamDeleted";
    m_updated = true;
    break;
  case FAMCreated:
    LOG(DEBUG) << "WsMonitor::signal() : Change occured on  " << filename << " - Code FamCreated";
    m_updated = true;
    break;
  }
  return SUCCESS;
}

int WsMonitor::signalConf(const string& filename, const int& code)
{
  string rootpath = WsGlobalProperties::instance()->get("global", "root_path", ".");
  string temp = rootpath + GlobalConfig::PathToLockFile;
  temp = boost::filesystem::path(temp).string();
  ifstream fconf(temp.c_str());
  if (fconf.good()) {
    LOG(DEBUG) << "WsMonitor::signalConf() : Lock File is present. Not updating";
    return SUCCESS;
  }
  switch (code) {
  case FAMDeleted:
    LOG(DEBUG) << "WsMonitor::signalConf() : Change occured on conf dir" << filename << " - Code FamDeleted";
    m_updated = true;
    break;
  case FAMCreated:
    LOG(DEBUG) << "WsMonitor::signalConf() : Change occured on conf dir" << filename << " - Code FamCreated";
    m_updated = true;
    break;
  case FAMChanged:
    LOG(DEBUG) << "WsMonitor::signalConf() : Change occured on conf dir " << filename << " - Code FamChanged";
    m_updated = true;
    break;
  }
}

