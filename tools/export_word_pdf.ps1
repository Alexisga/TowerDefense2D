param([Parameter(Mandatory=$true)][string]$InputDocx,[Parameter(Mandatory=$true)][string]$OutputPdf)
$ErrorActionPreference='Stop'
$word=$null
$document=$null
try {
    # Conversión documental por API COM; no simula entrada de teclado ni ratón.
    $word=New-Object -ComObject Word.Application
    $word.Visible=$false
    $word.DisplayAlerts=0
    $word.AutomationSecurity=3
    $document=$word.Documents.Open([System.IO.Path]::GetFullPath($InputDocx),$false,$true)
    $document.ExportAsFixedFormat([System.IO.Path]::GetFullPath($OutputPdf),17)
    Write-Output "WORD_PDF_OK pages=$($document.ComputeStatistics(2))"
} finally {
    if($null -ne $document){$document.Close(0);[void][Runtime.InteropServices.Marshal]::ReleaseComObject($document)}
    if($null -ne $word){$word.Quit();[void][Runtime.InteropServices.Marshal]::ReleaseComObject($word)}
}
