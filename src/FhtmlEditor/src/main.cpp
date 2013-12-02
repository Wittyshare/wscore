/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
 */
#include <Wt/WContainerWidget>
#include <Wt/WOverlayLoadingIndicator>
#include <Wt/WTheme>

#include <gdwtcore/gdApplication.h>
#include <gdwtcore/gdFHtmlEditor.h>

#include <ConfigManager/WsLayoutProperties.h>

using namespace Wt;

WApplication* createApplication(const WEnvironment& env)
{
  gdApplication* gdApp = new gdApplication(env);
  wApp->root()->addStyleClass("gdMainWin");
  // Chargement du fichier de resources .xml
  wApp->messageResourceBundle().use(wApp->docRoot() + wApp->resourcesUrl() + "/FtmlEditor/Transl/fhtmlEditor");
  // Chargement du fichier de configuration .xml
  wApp->messageResourceBundle().use("/etc/wt/fhtmlEditor_config");
  wApp->setTitle(WMessage::tr("win-title"));
  wApp->useStyleSheet(wApp->theme()->resourcesUrl() + "/FhtmlEditor/Css/fhtmlEditor.css");
  wApp->setCssTheme(WsLayoutProperties::instance()->get("global", "css_theme", "") );
  WApplication::instance()->setLoadingIndicator(new WOverlayLoadingIndicator());
  wApp->root()->resize(WLength(100, WLength::Percentage), WLength(100, WLength::Percentage));
  wApp->root()->addWidget(new gdFHtmlEditor());
  wApp->root()->setOverflow(WContainerWidget::OverflowHidden);
  return gdApp;
}

int main(int argc, char** argv)
{
  return WRun(argc, argv, &createApplication);
}

