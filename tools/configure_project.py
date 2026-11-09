from pathlib import Path
import xml.etree.ElementTree as ET

ns='http://schemas.microsoft.com/developer/msbuild/2003'
ET.register_namespace('',ns)
p=Path('TowerDefense2D/TowerDefense2D.vcxproj')
tree=ET.parse(p);root=tree.getroot()
q=lambda s:f'{{{ns}}}{s}'
for group in root.findall(q('ItemGroup')):
    for item in list(group):
        if item.tag in (q('ClCompile'),q('ClInclude')): group.remove(item)
group=ET.Element(q('ItemGroup'))
ET.SubElement(group,q('ClCompile'),Include='main.cpp')
for f in sorted(Path('src').glob('*.cpp')): ET.SubElement(group,q('ClCompile'),Include='..\\'+str(f).replace('/','\\'))
for f in sorted(Path('include').glob('*.h')): ET.SubElement(group,q('ClInclude'),Include='..\\'+str(f).replace('/','\\'))
root.insert(-3,group)
props=ET.Element(q('ItemDefinitionGroup'))
cc=ET.SubElement(props,q('ClCompile'))
ET.SubElement(cc,q('AdditionalIncludeDirectories')).text='$(ProjectDir)..\\include;%(AdditionalIncludeDirectories)'
ET.SubElement(cc,q('AdditionalOptions')).text='/utf-8 %(AdditionalOptions)'
ET.SubElement(cc,q('RuntimeLibrary')).text='MultiThreadedDLL'
root.insert(-3,props)
work=ET.Element(q('PropertyGroup'))
ET.SubElement(work,q('LocalDebuggerWorkingDirectory')).text='$(ProjectDir)..'
root.insert(-3,work)
ET.indent(tree,space='  ');tree.write(p,encoding='utf-8',xml_declaration=True)
filters=ET.Element(q('Project'),ToolsVersion='4.0')
fg=ET.SubElement(filters,q('ItemGroup'))
for name in ['src','include']:ET.SubElement(fg,q('Filter'),Include=name)
fg=ET.SubElement(filters,q('ItemGroup'))
for item in group:
    e=ET.SubElement(fg,item.tag,Include=item.attrib['Include'])
    ET.SubElement(e,q('Filter')).text='include' if item.tag==q('ClInclude') else 'src'
ET.indent(filters,space='  ');ET.ElementTree(filters).write(str(p)+'.filters',encoding='utf-8',xml_declaration=True)
