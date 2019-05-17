$relativePathToReleaseFolder = 'DeveMultiCompressor\bin\Release\netcoreapp2.2\win10-x64\Publish'

$invocation = (Get-Variable MyInvocation).Value
$directorypath = Split-Path $invocation.MyCommand.Path
$SolutionRoot = Split-Path -Path $directorypath -Parent

$joinedPath = Join-Path $SolutionRoot $relativePathToReleaseFolder
$totalOutputPath = Join-Path $directoryPath "Output\DeveMultiCompressor.7z"

Push-Location $joinedPath
7z a -t7z -m0=LZMA2 -mmt=on -mx9 -md=1536m -mfb=273 -ms=on -mqs=on -sccUTF-8 "$totalOutputPath" "**"
Pop-Location