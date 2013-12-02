cmake_minimum_required(VERSION 2.8.3)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${HARU_PREFIX} ${GDWT_PREFIX} ${GDCORE_PREFIX} ${GDWTCORE_PREFIX} ${WS_PREFIX})

SET ( APP_LIBRARIES
    ${APP_LIBRARIES}
    WsModule
)
SUBDIRS (
  WsModAnchor
  WsModBanners
  WsModBreadCrumb
  WsModDirectoryView2
  WsModFileUpload
  WsModFollowUs
  WsModGalleria
  WsModImages2
  WsModImagesGallery
  WsModLatestNews2
  WsModLinksList
  WsModMeetingView
  WsModMenu
  WsModMenuImages
  WsModNewsLetter
  WsModOdf
  WsModPdf
  WsModRSS
  WsModTwitter
  WsModVideo
  WsModZip
)
