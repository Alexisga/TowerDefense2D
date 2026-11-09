"""Actualiza solo los apartados 8 a 10 del Word entregado y verifica el resto."""
from pathlib import Path
from copy import deepcopy
import json,subprocess,zipfile,hashlib
from lxml import etree
from docx import Document
from docx.shared import Inches,Pt,RGBColor
from docx.oxml import OxmlElement
from docx.oxml.ns import qn

ROOT=Path(__file__).resolve().parents[1]
SOURCE=Path(r'C:/Users/User/OneDrive/Escritorio/7mo ciclo/Programación de Videojuegos con C++/Proyecto/APF2/APF2_Tower_Defense.docx')
backup=ROOT/'docs/backup/APF2_Tower_Defense_original_2026-11-09.docx'
if backup.exists():SOURCE=backup
OUTPUT=ROOT/'docs/APF2_Tower_Defense_actualizado.docx'
doc=Document(SOURCE);body=doc._element.body
nodes=list(body)
def text_of(el):return ''.join(el.itertext()) if False else ''.join(el.xpath('.//w:t/text()'))
start=next(i for i,n in enumerate(nodes) if n.tag==qn('w:p') and text_of(n)=='8. Código y documentación por Sprint')
end=next(i for i,n in enumerate(nodes) if n.tag==qn('w:p') and text_of(n)=='11. Stoppers, bugs y soluciones')
prefix=[etree.tostring(n,method='c14n') for n in nodes[:start]]
suffix=[etree.tostring(n,method='c14n') for n in nodes[end:]]
boundary=deepcopy(nodes[end-1])
assert boundary.xpath('.//w:sectPr'),'Se requiere conservar el salto de sección original'
original_ids={id(n) for n in body}

def para(text='',bold=False,size=11.5):
    p=doc.add_paragraph();p.paragraph_format.space_after=Pt(4);p.paragraph_format.line_spacing=1.05
    r=p.add_run(text);r.bold=bold;r.font.name='Times New Roman';r.font.size=Pt(size);r.font.color.rgb=RGBColor(0,0,0)
    return p
def heading(text,level=2,newpage=False):
    p=doc.add_paragraph(text,style=f'Heading {level}')
    p.paragraph_format.page_break_before=newpage;p.paragraph_format.keep_with_next=True
    p.paragraph_format.space_before=Pt(12 if level>1 else 0);p.paragraph_format.space_after=Pt(8)
    for r in p.runs:r.font.name='Times New Roman';r.font.size=Pt(13 if level==1 else 12);r.font.bold=True;r.font.color.rgb=RGBColor(0,0,0)
    return p
def table(number,title,headers,rows,widths):
    para(f'Tabla {number}',True,11);para(title,False,11)
    t=doc.add_table(rows=1,cols=len(headers));t.autofit=False
    for i,w in enumerate(widths):t.columns[i].width=Inches(w)
    for i,s in enumerate(headers):t.rows[0].cells[i].text=s
    for row in rows:
        cells=t.add_row().cells
        for i,s in enumerate(row):cells[i].text=str(s)
    for ri,row in enumerate(t.rows):
        pr=row._tr.get_or_add_trPr();pr.append(OxmlElement('w:cantSplit'))
        if ri==0:pr.append(OxmlElement('w:tblHeader'))
        for i,c in enumerate(row.cells):
            c.width=Inches(widths[i])
            tcpr=c._tc.get_or_add_tcPr()
            borders=OxmlElement('w:tcBorders')
            for side in ['top','left','bottom','right']:
                b=OxmlElement('w:'+side);b.set(qn('w:val'),'single');b.set(qn('w:sz'),'4');b.set(qn('w:color'),'B8B8B8');borders.append(b)
            tcpr.append(borders)
            if ri==0:
                sh=OxmlElement('w:shd');sh.set(qn('w:fill'),'E9EEEA');tcpr.append(sh)
            for p in c.paragraphs:
                p.paragraph_format.space_after=Pt(3);p.paragraph_format.space_before=Pt(3);p.paragraph_format.line_spacing=1.05
                for r in p.runs:r.font.name='Times New Roman';r.font.size=Pt(10);r.bold=ri==0
    para('',size=3)
    return t
def code(s):
    p=para(s,size=9);p.paragraph_format.keep_together=True
    for r in p.runs:r.font.name='Consolas'
    shade=OxmlElement('w:shd');shade.set(qn('w:fill'),'F1F1F1');p._p.get_or_add_pPr().append(shade)
def picture(filename,caption):
    p=doc.add_paragraph();p.paragraph_format.keep_with_next=True
    p.add_run().add_picture(str(ROOT/'docs/evidence'/filename),width=Inches(5.7))
    para(caption,size=10)

heading('8. Código y documentación por Sprint',1,True)
para('Avance funcional verificado al 9 de noviembre de 2026. Se continuó el proyecto existente de Visual Studio y se integraron las nueve clases previstas, los quince requerimientos funcionales y el registro CSV. La compilación y las pruebas se ejecutaron por herramientas de Codex bajo solicitud de Kevin; no constituyen aceptación académica ni pruebas manuales atribuidas a los estudiantes.')
para('Las secciones 1 a 7 mantienen su carácter de diseño y planificación. Los trabajos siguientes implementan anticipadamente parte de los sprints 5 y 6; no alteran el cronograma ni acreditan su cierre formal. Las secciones 11 y 12 conservan las fichas originales pendientes de cierre.')
heading('8.1 Ficha del Sprint')
table(25,'Ficha del avance de implementación',['Campo','Detalle'],[
('Periodo real','2026-11-09. Incrementos auténticos en develop; sin retrofechar trabajo ni commits.'),
('Responsabilidad','Kevin solicita y coordina esta etapa. Codex ejecuta código, compilaciones y pruebas mediante herramientas. Naomy y Diego conservan responsabilidades previstas; no se les atribuye ejecución no realizada.'),
('Sprint 4 Núcleo del juego','Mapa, waypoints, enemigos, oleadas, selección y validación de construcción. RF-01 a RF-05 implementados y comprobados.'),
('Sprint 5 Combate y progresión','Alcance, objetivo cercano, intervalos, proyectiles, daño, retiro, economía y resultados. RF-06 a RF-13 implementados y comprobados; ajuste de dificultad con jugadores pendiente.'),
('Sprint 6 Estados CSV e integración','Pausa, reanudación, reinicio y ocho eventos CSV implementados y comprobados. Medición local realizada; laboratorio, latencia visual de 100 ms y aceptación humana pendientes.'),
('Resultado','Versión jugable y demostrable. Sin aprobación docente ni cierre formal de sprints. Faltan recursos originales de Figma y verificaciones no funcionales indicadas en 8.3.')],[1.5,5.0])
heading('8.2 Porciones de código implementadas')
table(26,'Módulos y trazabilidad del código',['Archivo o método','Responsabilidad','RF','Commit'],[
('main.cpp','Inicializa Raylib y Game; entrada, actualización, dibujo y cierre. Directorio reproducible al abrir el EXE.','Soporte','51a17f4'),
('Game::TryBuild y Update','Coordina estados, economía, liquidación de entidades y fin de partida.','03–05, 09–15','7734384 / 51a17f4'),
('GameMap::SlotAt y Waypoints','Trece casillas permitidas fuera del camino y ruta fija.','01, 04','7734384'),
('Enemy::Update y TakeDamage','Consume distancia sobrante al girar, conserva vida y distingue eliminación de escape.','01, 08–11','7734384'),
('WaveManager::Update','Genera enemigos, espera campo vacío y aumenta cantidad, vida y velocidad.','02, 12, 13','7734384'),
('Tower::FindTarget y Update','Enemigo activo más cercano al disparar, alcance e intervalo por tipo.','03, 06, 07','7734384'),
('Projectile::Update','Sigue ID estable, aplica daño una sola vez y descarta objetivos inválidos.','07–09','7734384'),
('GameUI y AssetManager','HUD, tarjetas, previsualización, mensajes, menú y resultados; carga y liberación de sprites.','03, 04, 11, 12, 14, 15','7734384 / 51a17f4'),
('CsvLogger::Write','Encabezado, escape, acumulación, UTC y errores de escritura.','A-09 CSV','7734384')],[1.5,2.8,0.8,1.4])
para('GameTypes.h contiene enumerados y estructuras auxiliares; no es una décima clase. Los atributos de las nueve clases son privados y las consultas exponen vistas const. Los archivos se ubican físicamente en include/ y src/, con referencias en el proyecto y sus filtros.')
heading('Construcción y economía',3)
code('if (slot < 0) { return false; }\nif (Occupied(slot)) { return false; }\nif (coins_ < stats.cost) { return false; }\ntowers_.emplace_back(selection_, map_.BuildSlots()[slot], slot);\ncoins_ -= stats.cost;')
para('Extracto de Game::TryBuild, omitiendo únicamente mensajes y registro. Todas las validaciones preceden a la mutación: un intento rechazado no agrega torres ni modifica las monedas. La construcción se ajusta al centro de la casilla elegida y registra TORRE_CONSTRUIDA después de cobrar.')
heading('Pausa y estados',3)
code('if (state_ != GameState::ACTIVA || !std::isfinite(dt) || dt <= 0)\n    return;\nactiveTime_ += dt;')
para('Extracto de Game::Update. En PAUSADA no avanzan enemigos, proyectiles, temporizadores de torres, generación ni tiempo activo. HandleInput y Draw siguen disponibles para reanudar. Al reiniciar desde VICTORIA o DERROTA se vacían entidades, se restauran 180 monedas y 20 de vida y se asigna otro partida_id.')
heading('Objetivos e impactos',3)
para('Tower::FindTarget compara distancias cuadradas y descarta enemigos inactivos o fuera de alcance. Cada proyectil almacena un ID de objetivo y lo resuelve al actualizar; no conserva punteros a posiciones de un vector. Un disparo emitido persigue al objetivo aunque después salga del alcance; si el enemigo muere o escapa, se descarta sin daño adicional. Game liquida una recompensa o un daño de base por entidad y la retira en ese mismo cuadro.')
heading('Configuración y recursos',3)
para('Visual Studio 2026 Community, MSVC v145, C++20 y NuGet raylib 6.2.0. Resolución 1280 × 720 y MSAA x4. Debug usa /MDd y excluye MSVCRT de release aportada por la biblioteca estática C; Release usa /MD. Ambos ejecutables se compilaron y enlazaron en Windows x64.')
para('Parámetros provisionales de GameTypes.h: 20 de vida, 180 monedas; torre básica 50 / 12 daño / 120 px / 0,8 s; largo alcance 90 / 20 / 200 px / 1,4 s; recompensa 10 y daño de escape 1. Ocho oleadas: 6 + 2 × (n−1) enemigos, vida 32 + 12 × (n−1), velocidad 40 + 3 × (n−1) px/s. Aparición cada 0,85 s y espera de 5 s con el campo vacío.')
para('AssetManager dibuja sprites temporales independientes de primitivas 2D y los convierte en texturas al iniciar. No usa capturas completas de mockups como interfaz. Faltan las exportaciones originales de Figma. Se conserva la distribución del diseño y la paleta medieval, sin afirmar fidelidad exacta. Georgia y Segoe UI se cargan desde Windows si existen, con fuente de reserva; no se redistribuyen.')
heading('8.3 Pruebas y resultado del Sprint')
table(27,'Pruebas efectivamente ejecutadas',['Prueba','Resultado obtenido','Estado','Evidencia'],[
('Compilación x64','Debug y Release compilados y enlazados por MSBuild 18.10.1.','Aprobada','build-debug.log / build-release.log'),
('Reglas e integración','48 comprobaciones, cero fallos. Casillas, cobros, daño, intervalos, escapes, recompensa única y reinicios.','Aprobada','functional-tests.txt; tests/tests.cpp'),
('Campaña real','Ocho oleadas, 104 eliminaciones, vida 20, trece torres; victoria a 232,987 s simulados. Estrategia automática.','Aprobada','default-campaign.csv de QA; muestra CSV'),
('Pausa','Posiciones, vida, saldo, proyectiles, oleada y temporizadores congelados; reanudación conserva datos.','Aprobada','Batería RF-14; filas reales PAUSA/REANUDACION a 68,341 s'),
('CSV','Parser confirma 159 filas, diez columnas y ocho eventos. Escape, acumulación y errores controlados.','Aprobada','security-and-csv.json; eventos-muestra.csv'),
('Interfaz','Menú, construcción, rechazo, pausa, reanudación y resultados ejecutados y observados.','Verificada por herramientas','Capturas 01–11 en docs/evidence')],[1.1,3.0,0.9,1.5])
para('Los archivos de evidencia están en docs/evidence del repositorio. Las capturas de resultados usan configuraciones de prueba para alcanzar victoria o derrota; la campaña con parámetros predeterminados se verifica por separado. El recorrido nativo de Windows es automatizado por Codex, no una sesión manual de un estudiante.')
heading('Rendimiento y límites de la medición',3)
b=json.loads((ROOT/'docs/evidence/benchmark.json').read_text())
para(f'Equipo local: Intel Core i5-13400F, 10 núcleos y 16 hilos; NVIDIA GeForce RTX 3060, controlador 580.97, OpenGL 3.3. Se midieron {b["frames"]} cuadros tras 120 de calentamiento, con al menos {b["min_entities"]} entidades simultáneas, 1280 × 720 y MSAA x4. El escenario contiene 70 enemigos de vida alta y trece torres, además de proyectiles.')
para('Resultados reales: 59,7844 FPS medios; tiempo medio 16,7268 ms; percentil 95 de 16,7657 ms; percentil 99 de 17,0838 ms; máximo 80,5197 ms. Máximo medido de actualización y envío de dibujo en CPU: 6,9559 ms, sin incluir la espera de EndDrawing ni la finalización de GPU. Tiempos crudos en frame-times.csv y resumen en benchmark.json.')
para('RNF-01 queda parcialmente validado: la medición local no acredita 60 FPS estables en todos los cuadros ni el equipo del laboratorio. RNF-07 permanece pendiente de medición entre entrada física y respuesta visible; SetTargetFPS y los tiempos de CPU no prueban por sí solos el umbral de 100 ms. RNF-04 requiere aceptación humana de usabilidad. RNF-02, 03, 05, 06 y 09 tienen evidencia estructural, funcional y de ejecución; la revisión y sustentación del equipo siguen pendientes.')
heading('Incidencias y decisiones registradas',3)
para('El enlazador aislado quedó bloqueado en MSPDB140; se ejecutó fuera del aislamiento y se corrigió el conflicto de runtime Debug. Una aserción del fixture CSV se corrigió por la diferencia CRLF/LF de Windows. Una ejecución desde otro directorio reveló un CSV no escribible; main ahora establece la raíz del proyecto o el directorio del EXE. La segunda ejecución escribió eventos y mostró errores de construcción correctamente. Las soluciones y pruebas están en BITACORA.md y VALIDACION.md.')
para('No se implementaron características fuera del alcance: multijugador, A*, venta o mejora de torres, reparación ni guardado/carga. Pendientes: sprites originales, latencia visual, laboratorio, pruebas con jugadores, revisión humana y aceptación docente. CSV no implementa rotación automática ni bloqueo entre procesos; no se deben compartir sus escrituras entre dos instancias.')
heading('8.4 Evidencias de ejecución',2,True)
picture('02-partida.png','E-01. Partida real dibujada con Raylib. Los controles y entidades son funcionales; los sprites son temporales.')
picture('07-colocacion-valida.png','E-02. Previsualización en casilla válida, con alcance y costo. Renderizado automático del mismo código de presentación.')
heading('Evidencias de estados',2,True)
picture('03-pausa.png','E-03. Pausa con partida visible bajo la superposición. La prueba RF-14 comprueba la congelación del estado.')
picture('04-victoria.png','E-04. Victoria alcanzada por simulación con configuración de QA. La campaña predeterminada se verifica aparte.')
para('Menú, derrota, construcción inválida, carga de estrés y recorrido nativo de Windows se conservan en las demás capturas de docs/evidence. No se presentan los mockups como producto ejecutado.')

heading('9. GitHub y control de versiones',1,True)
para('Se creó y verificó el repositorio privado oficial del proyecto. El historial conserva el estado original y los incrementos reales de implementación y validación. Los commits ejecutados por Codex usan su identidad explícita; no se fabrican aportes del equipo.')
heading('9.1 Repositorio')
table(28,'Repositorio comprobado',['Campo','Detalle'],[
('Plataforma y propietario','GitHub; cuenta Alexisga, correspondiente a Kevin Jhans Alvarado Garayar.'),
('Repositorio y URL','TowerDefense2D\nhttps://github.com/Alexisga/TowerDefense2D'),
('Visibilidad','Privado, comprobado por API después de crearlo el 2026-11-09.'),
('Permisos comprobados','La cuenta conectada tiene admin, maintain, push, pull y triage. No se invitó a otros integrantes ni se cambió su acceso.'),
('Respaldo','Commits locales y push al remoto privado. Copia inicial de cuatro archivos en docs/backup, fuera de Git. El Word original se conserva como fuente y se entrega una copia actualizada.'),
('Retención del CSV','Los CSV de uso local se excluyen de Git; la muestra técnica de QA se versiona explícitamente. Respaldo manual con el juego cerrado; no hay rotación ni respaldo automático.')],[1.5,5])
heading('9.2 Estrategia de ramas')
table(29,'Estrategia realmente aplicada',['Rama','Uso real','Protección'],[
('main','Estado estable. El primer commit conserva el proyecto original; la integración de la versión validada se identifica en el historial final.','No configurada. Sin aprobación humana ni CI obligatoria.'),
('develop','Implementación y pruebas de esta etapa, con commits y push auténticos.','No configurada. Validación automatizada local antes de integrar.')],[1,3.2,2.3])
para('No se crearon ramas feature ni hotfix innecesarias. La guía de Semana 13 se aplica manteniendo cambios en develop hasta validar, comentarios breves y responsabilidades identificables. No se afirma una política de PR obligatorio o doble aprobación que no haya sido configurada. La integración y el estado remoto se documentan en el historial final.')
heading('9.3 Control de cambios')
commits=subprocess.check_output(['git','log','--reverse','--format=%h|%ad|%s','--date=short'],cwd=ROOT,text=True,encoding='utf-8').strip().splitlines()
rows=[]
for line in commits:
    sha,date,msg=line.split('|',2);rows.append((sha,date,msg,'Codex'))
table(30,'Commits reales anteriores a esta actualización documental',['Commit','Fecha','Cambio','Ejecución'],rows,[0.8,0.9,3.9,0.9])
para('Los hashes anteriores son referencias reales. La documentación final se añade en un commit posterior; su identificador debe consultarse en git log. No se inventan tags, versiones publicadas, PR revisados ni aprobaciones. El correo global del estudiante se comprobó en la inspección, pero los commits automatizados se realizaron con identidad Codex para evitar atribución ficticia.')

heading('10. Seguridad de credenciales',1,True)
para('El videojuego es local y no autentica jugadores ni se conecta a bases de datos o servicios externos. El CSV contiene eventos técnicos, sin nombres, correos ni contraseñas del jugador. La autenticación de GitHub pertenece a las herramientas del usuario y no se incorpora al juego.')
heading('10.1 Mecanismos utilizados')
para('Aplicados y comprobados: .gitignore excluye .vs, paquetes restaurables, binarios, archivos de usuario, .env, claves/certificados y archivos de credenciales; el repositorio es privado; se verificó el permiso de la cuenta; se analizaron patrones de secretos en el código y la muestra de registros. Las pruebas de CsvLogger comprueban formato, encabezado incompatible y errores de acceso.')
para('No aplican al juego: cuentas de servicio, contraseñas de jugadores, certificados TLS o secretos por ambiente. No se configura un Vault ni se afirma cifrado o rotación inexistentes. Las credenciales de GitHub se administran fuera del repositorio; para revocar el acceso se utiliza la configuración de aplicaciones autorizadas del proveedor. No se inspeccionó ni publicó material de autenticación.')
heading('10.2 Registro de configuración segura')
table(31,'Controles realmente configurados',['Elemento','Mecanismo y estado','Responsabilidad'],[
('Juego y CSV','Sin credenciales de aplicación ni datos personales del jugador. CSV acumulativo local; sin cifrado ni bloqueo de instancias.','Kevin; pruebas ejecutadas por Codex.'),
('Acceso a GitHub','Autorización externa completada por el usuario. Cuenta Alexisga y repositorio privado comprobados. Ningún token se guarda en código.','Usuario titular de la cuenta.'),
('Archivos excluidos','.gitignore versionado. Binarios, .env, archivos .key/.pem/.pfx/.p12 y credenciales JSON excluidos por patrones.','Codex configuró bajo solicitud de Kevin.'),
('Protecciones remotas','Protección de rama, aprobación por terceros, CI y escaneo continuo no configurados. Se registra el límite.','Decisión posterior del propietario.')],[1.2,3.9,1.4])
heading('10.3 Evidencia de no exposición',newpage=True)
security=json.loads((ROOT/'docs/evidence/security-and-csv.json').read_text(encoding='utf-8'))
table(32,'Revisión de seguridad y alcance de la evidencia',['Control','Método real','Resultado','Evidencia'],[
('Código y documentación','Búsqueda local de cuatro patrones: tokens GitHub, claves privadas, access keys AWS y asignaciones de secretos.','Cero hallazgos en '+str(security['files_scanned'])+' archivos. No es análisis exhaustivo ni Gitleaks.','security-and-csv.json; tools/verify_evidence.py'),
('Registros','Parser CSV de Python, encabezado exacto y valores no negativos; lectura del contenido de muestra.','159 filas, diez campos, ocho tipos de evento; sin campos de identidad del jugador.','eventos-muestra.csv'),
('Permisos','API de GitHub, consulta posterior a creación.','Repositorio privado; cuenta titular con permisos admin. No se acredita mínimo privilegio de otros usuarios.','Consulta real registrada en bitácora.'),
('Exclusiones','Reglas versionadas de .gitignore; archivos locales de usuario y datos fuera del índice.','Aplicadas. No se incluye configuración personal de Visual Studio ni paquetes restaurables.','.gitignore; git status')],[1,2.4,1.9,1.2])
para('La revisión de patrones se limita a include/, src/, main.cpp, README, .gitignore, docs/*.md y la muestra CSV. No inspecciona las credenciales del sistema ni constituye garantía de ausencia absoluta de secretos en todo el historial remoto. Se conserva el resultado técnico íntegro, con método y alcance, sin afirmar certificaciones ni controles no ejecutados.')

# Mover los elementos recién generados a la ubicación original sin reconstruir el resto.
new_nodes=[n for n in list(body) if id(n) not in original_ids]
for n in new_nodes:body.remove(n)
for n in nodes[start:end]:body.remove(n)
for i,n in enumerate(new_nodes+[boundary]):body.insert(start+i,n)
assert [etree.tostring(n,method='c14n') for n in list(body)[:start]]==prefix
assert [etree.tostring(n,method='c14n') for n in list(body)[start+len(new_nodes)+1:]]==suffix
doc.save(OUTPUT)
# Verificar que todos los recursos originales se conservan byte a byte.
with zipfile.ZipFile(SOURCE) as a,zipfile.ZipFile(OUTPUT) as b:
    media=[n for n in a.namelist() if n.startswith('word/media/')]
    assert all(a.read(n)==b.read(n) for n in media)
report={'source':str(SOURCE),'output':str(OUTPUT),'unchanged_prefix_nodes':len(prefix),'unchanged_suffix_nodes':len(suffix),'original_media_preserved':len(media),'new_body_nodes':len(new_nodes),'section_break_preserved':True}
(ROOT/'docs/evidence/document-preservation.json').write_text(json.dumps(report,ensure_ascii=False,indent=2),encoding='utf-8')
print(json.dumps(report,ensure_ascii=False,indent=2))
