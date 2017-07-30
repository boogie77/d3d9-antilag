import jinja2
import os
import os.path

os.chdir(os.path.dirname(__file__))
names = open('names.txt').read().splitlines()
loader = jinja2.FileSystemLoader('templates')
env = jinja2.Environment(loader=loader)

def render(filename):
    template = env.get_template(filename)
    output = template.render(names=names)
    with open(os.path.join('../src/genfiles', filename), 'w') as f:
        f.write(output)

render('export.def')
render('stub.asm')
render('loader.h')
