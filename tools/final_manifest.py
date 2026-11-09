from pathlib import Path
import hashlib,json,zipfile
from pypdf import PdfReader
root=Path(__file__).resolve().parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
source=root/'docs/backup/APF2_Tower_Defense_original_2026-11-09.docx'
updated=root/'docs/APF2_Tower_Defense_actualizado.docx'
reportpath=root/'docs/evidence/document-preservation.json'
r=json.loads(reportpath.read_text(encoding='utf-8'))
r.update({'source_backup':str(source),'source_sha256':sha(source),'output_sha256':sha(updated),'rendered_pages':len(PdfReader(root/'docs/qa/actualizado.pdf').pages),'visual_review':'68 páginas renderizadas inspeccionadas. Sin nuevas alteraciones en 1–7; índice general repaginado; 8–10 revisados e iterados.'})
reportpath.write_text(json.dumps(r,ensure_ascii=False,indent=2),encoding='utf-8')
report=json.loads((root/'docs/evidence/document-render-check.json').read_text())
assert report['changed_preserved_pages']==[3]
assert report['updated_pages']==68
with zipfile.ZipFile(source) as a,zipfile.ZipFile(updated) as b:
    media=[p for p in a.namelist() if p.startswith('word/media/')]
    assert len(media)==16
    assert all(a.read(p)==b.read(p) for p in media)
assert 'RESULT 48 passed, 0 failed' in (root/'docs/evidence/functional-tests.txt').read_text(encoding='utf-8')
print('FINAL_MANIFEST_OK: 68 paginas; 16 recursos originales; 48 comprobaciones aprobadas')
