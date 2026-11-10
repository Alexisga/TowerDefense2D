"""Instancia las fuentes OFL descargadas; no requiere instalarlas en Windows."""
from pathlib import Path
import sys
root=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(root/'build/fonttools'))
from fontTools.ttLib import TTFont
from fontTools.varLib.instancer import instantiateVariableFont
for source,target,axes in [
    ('Cinzel-Variable.ttf','Cinzel-Bold.ttf',{'wght':700}),
    ('Inter-Variable.ttf','Inter-Regular.ttf',{'wght':400,'opsz':24}),
    ('Inter-Variable.ttf','Inter-SemiBold.ttf',{'wght':600,'opsz':24}),
]:
    font=instantiateVariableFont(TTFont(root/'assets/fonts'/source),axes,inplace=False)
    font.save(root/'assets/fonts'/target)
    print(target)
