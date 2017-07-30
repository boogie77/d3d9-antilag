static inline void load_orig_ptrs(HMODULE orig_d3d9) {
{% for name in names %}
    extern FARPROC orig_{{ name }};
    orig_{{ name }} = GetProcAddress(orig_d3d9, "{{ name }}");
{% endfor %}
}
