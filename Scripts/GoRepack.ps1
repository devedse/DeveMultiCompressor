$ErrorActionPreference = "Stop"

$relativePathToReleaseFolder = '..\DeveImageOptimizerWPF\bin\Release'
$relativePathToILRepackExe = '..\packages\ILMerge.3.0.29\tools\net452\ILMerge.exe'
$fileNameOfPrimaryExe = 'DeveImageOptimizerWPF.exe'
$relativePathToOutputFolder = 'Output'

$invocation = (Get-Variable MyInvocation).Value
$directorypath = Split-Path $invocation.MyCommand.Path
$ilrepackexe = Join-Path $directorypath $relativePathToILRepackExe -Resolve

$releaseFolder = Join-Path $directorypath $relativePathToReleaseFolder -Resolve
$deveimageoptimizerexe = Join-Path $releaseFolder $fileNameOfPrimaryExe -Resolve
$outputfolder = Join-Path $directorypath $relativePathToOutputFolder
$outputexe = Join-Path $outputfolder $fileNameOfPrimaryExe

If(!(test-path $outputfolder))
{
	New-Item -ItemType Directory -Force -Path $outputfolder
}

Write-Host $directorypath;
Write-Host $ilrepackexe;

$arguments = @();

$arguments += "/lib:""C:\Program Files (x86)\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.7""";
$arguments += "/log";
$arguments += "/out:""$($outputexe)""";
$arguments += """$($deveimageoptimizerexe)""";

Get-ChildItem $releaseFolder -Filter *.dll | 
Foreach-Object {
	$path = """$($_.FullName)"""
	$arguments += $path
	Write-Host "Found dll for merging: $path"
}

Write-Host $arguments

& $ilrepackexe $arguments
if ($LastExitCode -ne 0) { $host.SetShouldExit($LastExitCode)  }