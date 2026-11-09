from pathlib import Path
import json, zipfile
from pypdf import PdfReader
from docx import Document

root = Path(r'C:/Users/User/OneDrive/Escritorio/7mo ciclo/Programación de Videojuegos con C++')
out = Path('docs/source-review')
out.mkdir(parents=True, exist_ok=True)
for path in [root/'Proyecto/APF2/APF2_Tower_Defense.pdf', *[root/f'Temas de clase/Semana {i}.pdf' for i in (11,12,13)]]:
    pdf = PdfReader(path).pages
    text = '\n'.join(f'\n=== PAGINA {i+1} ===\n{p.extract_text()}' for i,p in enumerate(pdf))
    (out/(path.stem+'.txt')).write_text(text, encoding='utf-8')
    print(path.name, len(pdf), 'paginas')
doc = Document(root/'Proyecto/APF2/APF2_Tower_Defense.docx')
lines=[]
for i,p in enumerate(doc.paragraphs):
    if p.text: lines.append(f'{i} [{p.style.name}] {p.text}')
for i,t in enumerate(doc.tables):
    lines.append(f'\nTABLA {i}\n'+'\n'.join(' | '.join(c.text for c in r.cells) for r in t.rows))
(out/'word.txt').write_text('\n'.join(lines),encoding='utf-8')
print('DOCX',len(doc.paragraphs),'parrafos',len(doc.tables),'tablas')
