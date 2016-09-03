rmdir "%~dp0Output" /s /q
del "%~dp0DeveMultiCompressor.7z"
del "%~dp0DeveMultiCompressor.zip"

"%~dp0..\packages\ILRepack.2.0.11\tools\ILRepack.exe" /out:"%~dp0Output\DeveMultiCompressor.exe" "%~dp0..\DeveMultiCompressor\bin\Release\DeveMultiCompressor.exe" "%~dp0..\DeveMultiCompressor\bin\Release\CommandLine.dll" "%~dp0..\DeveMultiCompressor\bin\Release\FSharp.Core.dll" "%~dp0..\DeveMultiCompressor\bin\Release\Newtonsoft.Json.dll"

cd "%~dp0Output"
dir

xcopy "%~dp0..\DeveMultiCompressor\bin\Release\Compressors" "Output\Compressors\" /e /y
xcopy "%~dp0..\DeveMultiCompressor\bin\Release\Precompressors" "Output\Precompressors\" /e /y

xcopy "%~dp0\SampleScripts" "Output\" /e /y

"%~dp0\7z_x64_1602\7za.exe" a -mm=Deflate -mfb=258 -mpass=15 "%~dp0DeveMultiCompressor.zip" ".\Output\*"
"%~dp0\7z_x64_1602\7za.exe" a -t7z -m0=LZMA2 -mmt=on -mx9 -md=1536m -mfb=273 -ms=on -mqs=on -sccUTF-8 "%~dp0DeveMultiCompressor.7z" ".\Output\*"