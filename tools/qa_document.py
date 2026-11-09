from pathlib import Path
import subprocess,hashlib,json
from pypdf import PdfReader
from PIL import Image,ImageOps,ImageDraw

root=Path(__file__).resolve().parents[1]
qa=root/'docs/qa'
poppler=Path(r'C:/Program Files/WindowsApps/OpenAI.CodexPrimaryRuntime.v26-1105-1297-0_26.1105.1297.0_x64__3k8sg7r9htsxt/dependencies/native/poppler/Library/bin/pdftoppm.exe')
for name in ['original','actualizado']:
    out=qa/name;out.mkdir(exist_ok=True)
    if name=='actualizado' or not list(out.glob('page-*.png')):
        subprocess.run([str(poppler),'-r','96','-png',str(qa/(name+'.pdf')),str(out/'page')],check=True)
    pdf=PdfReader(qa/(name+'.pdf'))
    heads=[]
    for i,p in enumerate(pdf.pages):
        t=p.extract_text()
        for h in ['8. Código y documentación por Sprint','9. GitHub y control de versiones','10. Seguridad de credenciales','11. Stoppers, bugs y soluciones','12. Evidencias, conclusiones y cierre']:
            if h in t:heads.append({'heading':h,'page':i+1})
    print(name,len(pdf.pages),heads)
orig=sorted((qa/'original').glob('page-*.png'))[:len(PdfReader(qa/'original.pdf').pages)]
new=sorted((qa/'actualizado').glob('page-*.png'))[:len(PdfReader(qa/'actualizado.pdf').pages)]
same=[];changed=[]
for i,(a,b) in enumerate(zip(orig[:54],new[:54]),1):
    if Image.open(a).tobytes()==Image.open(b).tobytes():same.append(i)
    else:changed.append(i)
report={'identical_preserved_pages':same,'changed_preserved_pages':changed,'original_pages':len(orig),'updated_pages':len(new),'renderer':'Microsoft Word ExportAsFixedFormat + bundled Poppler 96 dpi'}
(root/'docs/evidence/document-render-check.json').write_text(json.dumps(report,indent=2),encoding='utf-8')
print(report)
for start in range(0,len(new),6):
    thumb=Image.new('RGB',(1800,1650),'#dddddd');d=ImageDraw.Draw(thumb)
    for j,path in enumerate(new[start:start+6]):
        im=Image.open(path);im.thumbnail((590,775));x=(j%3)*600+(600-im.width)//2;y=(j//3)*825+25
        thumb.paste(im,(x,y));d.text(((j%3)*600+10,(j//3)*825+5),f'Pagina {start+j+1}',fill='black')
    thumb.save(qa/f'contact-{start+1:02}.jpg',quality=90)
