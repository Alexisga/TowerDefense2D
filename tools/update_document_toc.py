"""Actualiza únicamente números del índice general según el PDF revisado."""
from pathlib import Path
import json
from docx import Document
from pypdf import PdfReader
root=Path(__file__).resolve().parents[1]
path=root/'docs/APF2_Tower_Defense_actualizado.docx'
pdf=PdfReader(root/'docs/qa/actualizado.pdf')
headings=['8. Código y documentación por Sprint','9. GitHub y control de versiones','10. Seguridad de credenciales','11. Stoppers, bugs y soluciones','12. Evidencias, conclusiones y cierre']
pages={h:max(i+1 for i,p in enumerate(pdf.pages) if h in p.extract_text()) for h in headings}
doc=Document(path);edits=[]
for p in doc.paragraphs[:45]:
    for h,n in pages.items():
        if p.text.startswith(h+'\t'):
            for t in reversed(p._p.xpath('.//w:t')):
                if t.text and t.text.isdigit():
                    edits.append({'heading':h,'old':t.text,'new':str(n)});t.text=str(n);break
doc.save(path)
reportpath=root/'docs/evidence/document-preservation.json'
r=json.loads(reportpath.read_text(encoding='utf-8'));r['navigation_page_numbers_updated']=edits
reportpath.write_text(json.dumps(r,ensure_ascii=False,indent=2),encoding='utf-8')
print(edits)
