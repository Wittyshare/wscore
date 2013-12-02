#include <Wt/WApplication>
#include <Wt/WLogger>
#include <Wt/WText>

#include <Logger/WsLogger.h>

#include "WsModPdf.h"

using namespace Wt;

extern "C" {
  void WsModPdfInit(void)
  {
    LOG(DEBUG) << "WsModPdf :: Preparing required libraries : libwt.so";
    void* hndl = dlopen("libwt.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      LOG(ERROR) << "WsModPdf :: Cannot load libwt.so shared library! " << dlerror();
      return;
    }
  }
}

WsPdf::WsPdf(WContainerWidget* parent)
  : WContainerWidget(parent)
{ }

WsPdf::~WsPdf()
{ }

void WsPdf::setDiffPath(const std::string& diffPath)
{
  m_sDiffPath = diffPath;
}

void WsPdf::load()
{
  WContainerWidget::load();
  setId("pdf");
  resize(WLength(100, WLength::Percentage), WLength(100, WLength::Percentage));
  setOverflow(WContainerWidget::OverflowAuto);
  std::string  p1 = Wt::WApplication::instance()->internalPath();
  WApplication::instance()->log("notice") << "WsPdf::load() internalPath : " << p1; // /about/wt.pdf
  //std::string  p = "/demo_site/about/wt.pdf";
  std::string  p(m_sDiffPath + p1);
  WApplication::instance()->log("notice") << "WsPdf::load() diffPath : " << p;
  if ( p.size() > 0 ) {
    std::string javaScript = "\
            PDFJS.workerSrc='" + WApplication::instance()->resourcesUrl() + "webpdf/pdf.js';\
            PDFJS.getPdf('" + p + "', function getPdf(data) {\
                var pdf = new PDFJS.PDFDoc(data);\
                var page = pdf.getPage(1);\
                var scale = 1.5;\
                var canvas = document.getElementById('pdf');\
                var context = canvas.getContext('2d');\
                canvas.height = page.height * scale;\
                canvas.width  = page.width  * scale;\
                })";
    WApplication::instance()->log("notice") << "WsPdf::load() execute this javascript : " << javaScript;
    WApplication::instance()->doJavaScript(javaScript);
  }
}





WsModPdf::WsModPdf()
  : WsModule()
{
  // Add the required javascript file
  WApplication::instance()->require(WApplication::instance()->resourcesUrl() + "webpdf/pdf.js");
  LOG(DEBUG) << "end ctor of WsModPdf !";
}

WsModPdf::~WsModPdf()
{
  LOG(DEBUG) << "end dtor of WsModPdf !";
}

WWidget* WsModPdf::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModPdf::createContents(WContainerWidget* parent) const
{
  WsPdf* pcw = new WsPdf(parent);
  pcw->setDiffPath(diffPath());
  pcw->setOptions(options());
  return pcw;
}

WsEditorWidget* WsModPdf::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModPdf::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

bool WsModPdf::saveEditor() const
{
  return true;
}

std::string WsModPdf::description() const
{
  return "WsModPdf wittyShare module";
}

