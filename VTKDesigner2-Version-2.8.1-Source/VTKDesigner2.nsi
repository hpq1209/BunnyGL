; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "VTK Designer"
!define PRODUCT_VERSION "2.8.1"
!define PRODUCT_PUBLISHER "VCreate Logic Pvt. Ltd."
!define PRODUCT_WEB_SITE "http://www.vcreatelogic.com"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\VTKDesigner2.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "Main\icons\vtkdicon.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "License.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\VTKDesigner2.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "VTKDesigner2-Version-2.8.1-Setup.exe"
; InstallDir "$PROGRAMFILES\VTK Designer"
InstallDir "$LOCALAPPDATA\VTK Designer 2"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show
RequestExecutionLevel admin

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "bin\VTKSystem.dll"
  File "bin\VTKFullscreen.dll"
  File "bin\VisSystemNodeClassExplorer.dll"
  File "bin\VisSystemCanvas.dll"
  File "bin\Utils.dll"
  File "bin\TemplateManager.dll"
  File "bin\ScriptEditor.dll"
  File "bin\PropertyEditor.dll"
  File "bin\ProjectManager.dll"
  File "bin\MPRViewer.dll"
  File "bin\MessageLog.dll"
  File "bin\MainWindow.dll"
  File "bin\ImagingAlgorithms.dll"
  File "bin\GCF2.dll"
  File "bin\FractalAlgorithms.dll"
  File "bin\DataFileLoader.dll"
  File "bin\CodeGenerator.dll"
  File "bin\VTKDesigner2.exe"
  CreateDirectory "$SMPROGRAMS\VTK Designer"
  CreateShortCut "$SMPROGRAMS\VTK Designer\VTK Designer.lnk" "$INSTDIR\VTKDesigner2.exe"
  CreateShortCut "$DESKTOP\VTK Designer.lnk" "$INSTDIR\VTKDesigner2.exe"
  SetOutPath "$INSTDIR\Data"
  File "bin\Data\Terrain.jpg"
  SetOutPath "$INSTDIR\guixml"
  File "bin\guixml\VTKDesigner2.xml"
  File "bin\guixml\UtilsComponent.xml"
  File "bin\guixml\TemplateManagerComponent.xml"
  File "bin\guixml\ScriptEditorComponent.xml"
  File "bin\guixml\PropertyEditorComponent.xml"
  File "bin\guixml\ProjectManagerComponent.xml"
  File "bin\guixml\MPRViewerComponent.xml"
  File "bin\guixml\MessageLogComponent.xml"
  File "bin\guixml\MainWindowComponent.xml"
  File "bin\guixml\ImagingAlgorithmsComponent.xml"
  File "bin\guixml\FractalAlgorithmsComponent.xml"
  File "bin\guixml\CVtkVisSystemComponent.xml"
  File "bin\guixml\CVisSystemCanvasComponent.xml"
  File "bin\guixml\CVTKFullscreenComponent.xml"
  File "bin\guixml\CVisSystemNodeClassExplorerComponent.xml"
  File "bin\guixml\CVisSystemCanvasComponent.xml"
  File "bin\guixml\CodeGeneratorComponent.xml"
  File "bin\guixml\CDataFileLoaderComponent.xml"
  SetOutPath "$INSTDIR\Samples"
  File "bin\Samples\WedgeSourceDemo.vwproj"
  File "bin\Samples\WebSourceDemo.vwproj"
  File "bin\Samples\Trajectory.vwproj"
  File "bin\Samples\TPlane.vwproj"
  File "bin\Samples\Terrain.vwproj"
  File "bin\Samples\streamer.vwproj"
  File "bin\Samples\specularSpheres.vwproj"
  File "bin\Samples\SincFunction.vwproj"
  File "bin\Samples\SimpleRayCast.vwproj"
  File "bin\Samples\SierpinskiTriangles.vwproj"
  File "bin\Samples\SierpinskiPyramid.vwproj"
  File "bin\Samples\ShaderTest.vwproj"
  File "bin\Samples\probeWithPointWidget.vwproj"
  File "bin\Samples\MengerSquare.vwproj"
  File "bin\Samples\MengerSponge.vwproj"
  File "bin\Samples\MathFunctions.vwproj"
  File "bin\Samples\Mace.vwproj"
  File "bin\Samples\imageWarp.vwproj"
  File "bin\Samples\iceCream.vwproj"
  File "bin\Samples\Glyph3D.vwproj"
  File "bin\Samples\FractalTerrains.vwproj"
  File "bin\Samples\flamingo.vwproj"
  File "bin\Samples\finance.vwproj"
  File "bin\Samples\deciFran.vwproj"
  File "bin\Samples\cutCombustor.vwproj"
  File "bin\Samples\Cube.vwproj"
  File "bin\Samples\ContQuad.vwproj"
  File "bin\Samples\Cone.vwproj"
  File "bin\Samples\ColorSph.vwproj"
  File "bin\Samples\CellCenters.vwproj"
  File "bin\Samples\BezierWidget.vwproj"
  File "bin\Samples\BezierSurface.vwproj"
  SetOutPath "$INSTDIR\Shaders"
  File "bin\Shaders\XRayShader.xml"
  File "bin\Shaders\ToonShading.xml"
  File "bin\Shaders\Simple.xml"
  File "bin\Shaders\ProceduralTexture.xml"
  File "bin\Shaders\Mandlebrot.xml"
  File "bin\Shaders\ImageSaturation.xml"
  File "bin\Shaders\ImageContrast.xml"
  File "bin\Shaders\ImageBrightness.xml"
  File "bin\Shaders\HemisphereLight.xml"
  File "bin\Shaders\HatchShading.xml"
  File "bin\Shaders\GoochShading.xml"
  File "bin\Shaders\DirectionLight.xml"
  File "bin\Shaders\BumpMapping.xml"
  File "bin\Shaders\BrickShading.xml"
  File "bin\Shaders\BlurEffect.xml"
  File "bin\Shaders\Animation.xml"
  SetOutPath "$INSTDIR\Templates"
  File "bin\Templates\GridBasedFilter.vwt"
  File "bin\Templates\Basic.vwt"
  SetOutPath "$INSTDIR\"
  File "C:\VTK-5.8.0-VS2010-SDK\RELEASE\bin\vtkzlib.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkWidgets.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkVolumeRendering.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtksys.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkRendering.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkIO.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkImaging.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkGraphics.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkGenericFiltering.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkfreetype.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkFiltering.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkexpat.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkexoIIc.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkDICOMParser.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkCommon.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\QVTK.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\MapReduceMPI.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\mpistubs.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkalglib.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkCharts.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkftgl.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkGeovis.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkhdf5.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkHybrid.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkInfovis.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkjpeg.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtklibxml2.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkmetaio.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkNetCDF.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkNetCDF_cxx.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkpng.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkproj4.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtktiff.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkverdict.dll"
  File "C:\VTK-5.8.0-VS2010-SDK\release\bin\vtkViews.dll"
  File "C:\Qt\4.8.4\bin\QtCore4.dll"
  File "C:\Qt\4.8.4\bin\QtXmlPatterns4.dll"
  File "C:\Qt\4.8.4\bin\QtXml4.dll"
  File "C:\Qt\4.8.4\bin\QtWebKit4.dll"
  File "C:\Qt\4.8.4\bin\QtTest4.dll"
  File "C:\Qt\4.8.4\bin\QtSql4.dll"
  File "C:\Qt\4.8.4\bin\QtOpenGL4.dll"
  File "C:\Qt\4.8.4\bin\QtNetwork4.dll"
  File "C:\Qt\4.8.4\bin\QtScript4.dll"
  File "C:\Qt\4.8.4\bin\QtGui4.dll"
  File "C:\Qt\4.8.4\bin\QtDeclarative4.dll"
  File "C:\Qt\4.8.4\bin\QtDesignerComponents4.dll"
  File "C:\Qt\4.8.4\bin\QtDesigner4.dll"
  File "C:\Qt\4.8.4\bin\QtSvg4.dll"
  File "C:\Users\prashanthudupa\Downloads\vcredist_x86.exe"
  
  ExecWait "$INSTDIR\vcredist_x86.exe /q"
SectionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\VTK Designer\VTK Designer Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\VTK Designer\Uninstall VTK Designer 2.8.1.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\VTKDesigner2.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\VTKDesigner2.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtSvg4.dll"
  Delete "$INSTDIR\QtDesigner4.dll"
  Delete "$INSTDIR\QtDesignerComponents4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtDeclarative4.dll"
  Delete "$INSTDIR\QtScript4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\QtOpenGL4.dll"
  Delete "$INSTDIR\QtSql4.dll"
  Delete "$INSTDIR\QtTest4.dll"
  Delete "$INSTDIR\QtWebKit4.dll"
  Delete "$INSTDIR\QtXml4.dll"
  Delete "$INSTDIR\QtXmlPatterns4.dll"
  Delete "$INSTDIR\QVTK.dll"
  Delete "$INSTDIR\vtkCommon.dll"
  Delete "$INSTDIR\vtkDICOMParser.dll"
  Delete "$INSTDIR\vtkexoIIc.dll"
  Delete "$INSTDIR\vtkexpat.dll"
  Delete "$INSTDIR\vtkFiltering.dll"
  Delete "$INSTDIR\vtkfreetype.dll"
  Delete "$INSTDIR\vtkGenericFiltering.dll"
  Delete "$INSTDIR\vtkGraphics.dll"
  Delete "$INSTDIR\vtkImaging.dll"
  Delete "$INSTDIR\vtkIO.dll"
  Delete "$INSTDIR\vtkRendering.dll"
  Delete "$INSTDIR\vtksys.dll"
  Delete "$INSTDIR\vtkVolumeRendering.dll"
  Delete "$INSTDIR\vtkWidgets.dll"
  Delete "$INSTDIR\vtkzlib.dll"
  Delete "$INSTDIR\MapReduceMPI.dll"
  Delete "$INSTDIR\mpistubs.dll"
  Delete "$INSTDIR\vtkalglib.dll"
  Delete "$INSTDIR\vtkCharts.dll"
  Delete "$INSTDIR\vtkftgl.dll"
  Delete "$INSTDIR\vtkGeovis.dll"
  Delete "$INSTDIR\vtkhdf5.dll"
  Delete "$INSTDIR\vtkHybrid.dll"
  Delete "$INSTDIR\vtkInfovis.dll"
  Delete "$INSTDIR\vtkjpeg.dll"
  Delete "$INSTDIR\vtklibxml2.dll"
  Delete "$INSTDIR\vtkmetaio.dll"
  Delete "$INSTDIR\vtkNetCDF.dll"
  Delete "$INSTDIR\vtkNetCDF_cxx.dll"
  Delete "$INSTDIR\vtkpng.dll"
  Delete "$INSTDIR\vtkproj4.dll"
  Delete "$INSTDIR\vtktiff.dll"
  Delete "$INSTDIR\vtkverdict.dll"
  Delete "$INSTDIR\vtkViews.dll"
  Delete "$INSTDIR\Templates\Basic.vwt"
  Delete "$INSTDIR\Templates\GridBasedFilter.vwt"
  Delete "$INSTDIR\Shaders\Animation.xml"
  Delete "$INSTDIR\Shaders\BlurEffect.xml"
  Delete "$INSTDIR\Shaders\BrickShading.xml"
  Delete "$INSTDIR\Shaders\BumpMapping.xml"
  Delete "$INSTDIR\Shaders\DirectionLight.xml"
  Delete "$INSTDIR\Shaders\GoochShading.xml"
  Delete "$INSTDIR\Shaders\HatchShading.xml"
  Delete "$INSTDIR\Shaders\HemisphereLight.xml"
  Delete "$INSTDIR\Shaders\ImageBrightness.xml"
  Delete "$INSTDIR\Shaders\ImageContrast.xml"
  Delete "$INSTDIR\Shaders\ImageSaturation.xml"
  Delete "$INSTDIR\Shaders\Mandlebrot.xml"
  Delete "$INSTDIR\Shaders\ProceduralTexture.xml"
  Delete "$INSTDIR\Shaders\Simple.xml"
  Delete "$INSTDIR\Shaders\ToonShading.xml"
  Delete "$INSTDIR\Shaders\XRayShader.xml"
  Delete "$INSTDIR\Samples\BezierSurface.vwproj"
  Delete "$INSTDIR\Samples\BezierWidget.vwproj"
  Delete "$INSTDIR\Samples\CellCenters.vwproj"
  Delete "$INSTDIR\Samples\ColorSph.vwproj"
  Delete "$INSTDIR\Samples\Cone.vwproj"
  Delete "$INSTDIR\Samples\ContQuad.vwproj"
  Delete "$INSTDIR\Samples\Cube.vwproj"
  Delete "$INSTDIR\Samples\cutCombustor.vwproj"
  Delete "$INSTDIR\Samples\deciFran.vwproj"
  Delete "$INSTDIR\Samples\finance.vwproj"
  Delete "$INSTDIR\Samples\flamingo.vwproj"
  Delete "$INSTDIR\Samples\FractalTerrains.vwproj"
  Delete "$INSTDIR\Samples\Glyph3D.vwproj"
  Delete "$INSTDIR\Samples\iceCream.vwproj"
  Delete "$INSTDIR\Samples\imageWarp.vwproj"
  Delete "$INSTDIR\Samples\Mace.vwproj"
  Delete "$INSTDIR\Samples\MathFunctions.vwproj"
  Delete "$INSTDIR\Samples\MengerSponge.vwproj"
  Delete "$INSTDIR\Samples\MengerSquare.vwproj"
  Delete "$INSTDIR\Samples\probeWithPointWidget.vwproj"
  Delete "$INSTDIR\Samples\ShaderTest.vwproj"
  Delete "$INSTDIR\Samples\SierpinskiPyramid.vwproj"
  Delete "$INSTDIR\Samples\SierpinskiTriangles.vwproj"
  Delete "$INSTDIR\Samples\SimpleRayCast.vwproj"
  Delete "$INSTDIR\Samples\SincFunction.vwproj"
  Delete "$INSTDIR\Samples\specularSpheres.vwproj"
  Delete "$INSTDIR\Samples\streamer.vwproj"
  Delete "$INSTDIR\Samples\Terrain.vwproj"
  Delete "$INSTDIR\Samples\TPlane.vwproj"
  Delete "$INSTDIR\Samples\Trajectory.vwproj"
  Delete "$INSTDIR\Samples\WebSourceDemo.vwproj"
  Delete "$INSTDIR\Samples\WedgeSourceDemo.vwproj"
  Delete "$INSTDIR\guixml\CDataFileLoaderComponent.xml"
  Delete "$INSTDIR\guixml\CodeGeneratorComponent.xml"
  Delete "$INSTDIR\guixml\CVisSystemCanvasComponent.xml"
  Delete "$INSTDIR\guixml\CVisSystemNodeClassExplorerComponent.xml"
  Delete "$INSTDIR\guixml\CVTKFullscreenComponent.xml"
  Delete "$INSTDIR\guixml\CVtkVisSystemComponent.xml"
  Delete "$INSTDIR\guixml\CVisSystemCanvasComponent.xml"
  Delete "$INSTDIR\guixml\FractalAlgorithmsComponent.xml"
  Delete "$INSTDIR\guixml\ImagingAlgorithmsComponent.xml"
  Delete "$INSTDIR\guixml\MainWindowComponent.xml"
  Delete "$INSTDIR\guixml\MessageLogComponent.xml"
  Delete "$INSTDIR\guixml\MPRViewerComponent.xml"
  Delete "$INSTDIR\guixml\ProjectManagerComponent.xml"
  Delete "$INSTDIR\guixml\PropertyEditorComponent.xml"
  Delete "$INSTDIR\guixml\ScriptEditorComponent.xml"
  Delete "$INSTDIR\guixml\TemplateManagerComponent.xml"
  Delete "$INSTDIR\guixml\UtilsComponent.xml"
  Delete "$INSTDIR\guixml\VTKDesigner2.xml"
  Delete "$INSTDIR\Data\Terrain.jpg"
  Delete "$INSTDIR\VTKDesigner2.exe"
  Delete "$INSTDIR\CodeGenerator.dll"
  Delete "$INSTDIR\DataFileLoader.dll"
  Delete "$INSTDIR\FractalAlgorithms.dll"
  Delete "$INSTDIR\GCF2.dll"
  Delete "$INSTDIR\ImagingAlgorithms.dll"
  Delete "$INSTDIR\MainWindow.dll"
  Delete "$INSTDIR\MessageLog.dll"
  Delete "$INSTDIR\MPRViewer.dll"
  Delete "$INSTDIR\ProjectManager.dll"
  Delete "$INSTDIR\PropertyEditor.dll"
  Delete "$INSTDIR\ScriptEditor.dll"
  Delete "$INSTDIR\TemplateManager.dll"
  Delete "$INSTDIR\Utils.dll"
  Delete "$INSTDIR\VisSystemCanvas.dll"
  Delete "$INSTDIR\VisSystemNodeClassExplorer.dll"
  Delete "$INSTDIR\VTKFullscreen.dll"
  Delete "$INSTDIR\VTKSystem.dll"
  Delete "$INSTDIR\vcredist_x86.exe"

  Delete "$SMPROGRAMS\VTK Designer\Uninstall VTK Designer 2.8.1.lnk"
  Delete "$SMPROGRAMS\VTK Designer\VTK Designer Website.lnk"
  Delete "$DESKTOP\VTK Designer.lnk"
  Delete "$SMPROGRAMS\VTK Designer\VTK Designer.lnk"

  RMDir "$SMPROGRAMS\VTK Designer"
  RMDir "$INSTDIR\Templates"
  RMDir "$INSTDIR\Shaders"
  RMDir "$INSTDIR\Samples"
  RMDir "$INSTDIR\guixml"
  RMDir "$INSTDIR\Data"
  RMDir "$INSTDIR\"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd