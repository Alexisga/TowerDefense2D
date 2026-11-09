from pathlib import Path
import csv,json,re,hashlib,subprocess

root=Path(__file__).resolve().parents[1]
evidence=root/'docs/evidence'
header='fecha_hora partida_id tiempo_activo_s evento oleada vida_base monedas enemigos_eliminados resultado detalle'.split()
rows=[]
for name in ['default-campaign.csv','defeat.csv','pause.csv']:
    with (root/'tests/tmp'/name).open(encoding='utf-8',newline='') as f:
        reader=csv.DictReader(f)
        assert reader.fieldnames==header
        data=list(reader)
        latest=data[-1]['partida_id']
        # La prueba de derrota reinicia al final; conservar la partida finalizada.
        ends=[r['partida_id'] for r in data if r['evento']=='PARTIDA_FINALIZADA']
        if ends: latest=ends[-1]
        rows.extend(r for r in data if r['partida_id']==latest)
events={r['evento'] for r in rows}
expected={'INICIO_PARTIDA','INICIO_OLEADA','TORRE_CONSTRUIDA','ENEMIGO_ELIMINADO','BASE_DAÑADA','PAUSA','REANUDACION','PARTIDA_FINALIZADA'}
assert events==expected,events
assert all(int(r['vida_base'])>=0 and int(r['monedas'])>=0 and None not in r for r in rows)
with (evidence/'eventos-muestra.csv').open('w',encoding='utf-8',newline='') as f:
    w=csv.DictWriter(f,fieldnames=header);w.writeheader();w.writerows(rows)
# Analizar archivos del proyecto versionados y los nuevos módulos permitidos.
patterns={
    'github_token':re.compile(r'gh[pousr]_[A-Za-z0-9]{20,}|github_pat_[A-Za-z0-9_]{20,}'),
    'private_key':re.compile(r'-----BEGIN (?:RSA |EC |OPENSSH )?PRIVATE KEY-----'),
    'aws_access_key':re.compile(r'AKIA[A-Z0-9]{16}'),
    'assigned_secret':re.compile(r'''(?i)(?:password|passwd|api_key|secret|access_token)\s*[:=]\s*["'][^"'\s]{8,}["']'''),
}
findings=[];files=0
paths=[*root.glob('include/*.h'),*root.glob('src/*.cpp'),*root.glob('TowerDefense2D/*.cpp'),root/'README.md',root/'.gitignore',*root.glob('docs/*.md'),evidence/'eventos-muestra.csv']
for p in paths:
    files+=1;s=p.read_text(encoding='utf-8')
    for name,pattern in patterns.items():
        for m in pattern.finditer(s):findings.append({'file':p.relative_to(root).as_posix(),'line':s[:m.start()].count('\n')+1,'pattern':name})
report={'date':'2026-11-09','method':'Busqueda local automatizada de cuatro patrones; no es Gitleaks ni escaneo exhaustivo','files_scanned':files,'findings':findings,'csv_rows':len(rows),'csv_columns':header,'csv_event_types':sorted(events),'scope':'include, src, main.cpp, README, .gitignore, docs/*.md y muestra CSV; no revisa credenciales del sistema ni historial remoto'}
(evidence/'security-and-csv.json').write_text(json.dumps(report,ensure_ascii=False,indent=2),encoding='utf-8')
assert not findings,findings
print(json.dumps(report,ensure_ascii=False,indent=2))
