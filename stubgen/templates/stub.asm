section .text
{% for name in names if name != 'Direct3DCreate9' %}
global _{{ name }}
_{{ name }}:
    jmp dword [_orig_{{ name }}]
{% endfor %}

section .bss
{% for name in names %}
global _orig_{{ name }}
_orig_{{ name }}: resd 1
{% endfor %}
