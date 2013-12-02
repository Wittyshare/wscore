/*
 *
 *       Filename:  WsFsDaemonLoader.cpp
 *
 *    Description:
 *
 *        Created:  02/14/12 11:58:54
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */


#include "WsFsDaemonLoader.h"
#include <unistd.h>
#include <iostream>

#include <Logger/WsCrashLogger.h>

WsFsDaemonLoader::WsFsDaemonLoader(WsGlobalProperties* props, bool bdaemon):
  m_props(props),
  m_daemon(bdaemon)
{
}

WsFsDaemonLoader::~WsFsDaemonLoader()
{
  LOG(DEBUG) << "WsFsDaemonLoader::~WsFsDaemonLoader() : deleting this";
  delete m_server;
}

int WsFsDaemonLoader::start()
{
  if (!m_daemon) {
    m_server = new WsFsDaemon(m_props);
    return m_server->bind();
  } else
    return daemonize();
}

int WsFsDaemonLoader::daemonize()
{
  pid_t pid = fork();
  if (pid == 0) {
    m_server = new WsFsDaemon(m_props);
    return m_server->bind();
  } else {
    exit(0);
  }
}


void usage()
{
  cout << "Usage : " << endl;
  cout << "[-h | --host] value\t: connect on host" << endl;
  cout << "[-p | --port] value\t: connect on port" << endl;
  cout << "[--protocol]  value\t: use protocol" << endl;
  cout << "[--daemonize]\t\t: run server in background" << endl;
  cout << "[--root]\t\t: Filesystem root path" << endl;
  cout << "[--help]\t\t: display this help message" << endl;
  cout << "[--pid] value\t\t: write PID in file" << endl;
  exit(0);
}

WsFsDaemonLoader* serverLoader;

void handle_int(int sig)
{
  LOG(INFO) << "WsFsDaemonLoader::handle_int() : Stopping server";
  delete serverLoader;
  exit(sig);
}


int main(int argc, char** argv)
{
  signal( SIGINT, &handle_int);
  signal( SIGSEGV, &WsCrashLogger::handleSignal);
  signal( SIGABRT, &WsCrashLogger::handleSignal);
  signal( SIGILL,  &WsCrashLogger::handleSignal);
  signal( SIGFPE,  &WsCrashLogger::handleSignal);
  WsGlobalProperties* props = WsGlobalProperties::instance();
  static struct option long_options[] = {
    {"host", required_argument, NULL, 'h'},
    {"port", required_argument, NULL, 'p'},
    {"daemonize", no_argument, NULL, 0},
    {"protocol", required_argument, NULL, 1},
    {"help", no_argument, NULL, 2},
    {"root", required_argument, NULL, 3},
    {"pid", required_argument, NULL, 4},
    {NULL, 0, NULL, 0}
  };
  int option_index = 0;
  bool bdaemon = false;
  bool bpid = false;
  std::string pidPath = "";
  char c;
  while ((c = getopt_long (argc, argv, "h:p:01:", long_options, &option_index)) != -1) {
    switch (c) {
    case 'h':
      props->set("global", "host", string((char*)optarg));
      break;
    case 'p':
      props->set("global", "port", string((char*)optarg));
      break;
    case 0:
      bdaemon = true;
      break;
    case 1:
      props->set("global", "protocol", string((char*)optarg));
      break;
    case 2:
      usage();
      break;
    case 3:
      props->set("global", "root_path", string((char*)optarg));
      break;
    case 4:
      bpid = true;
      pidPath = string((char*)optarg);
      break;
    default:
      usage();
      exit(-1);
    }
  }
  /* Set apache www-data uid */
  /* setuid(33);*/
  serverLoader = new WsFsDaemonLoader(props, bdaemon);
  if (bpid) {
    ofstream myfile;
    myfile.open (pidPath.c_str());
    myfile << getpid();
    myfile.close();
  }
  /* Init the random number generator */
  srand (time(NULL));
  if (serverLoader->start() == SUCCESS)
    return EXIT_SUCCESS;
  else return EXIT_FAILURE;
}

