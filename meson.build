project('fuck monitor', 'c')

src = [ 'src/main.c', 'src/argos.c', 'src/delay.c', 'src/string.c', 'src/memory.c', 'src/cpu.c' , 'src/net.c', 'src/power.c']

add_global_arguments('-std=gnu11', language: 'c')
add_global_arguments('-Wextra', language: 'c')
add_global_arguments('-O2', language: 'c')

opt_debug = get_option('debug')
if opt_debug > 0
	debugstr = '-DDEBUG_ENABLE=@0@'
	debugstr = debugstr.format(opt_debug)
	add_global_arguments(debugstr, language: 'c')
endif

opt_debug_color = get_option('dolor')
if opt_debug_color > 0
	debugstr = '-DDEBUG_COLOR=@0@'
	debugstr = debugstr.format(opt_debug_color)
	add_global_arguments(debugstr, language: 'c')
endif

opt_assert = get_option('assert')
if opt_assert > 0
	add_global_arguments('-DIASSERT_ENABLE=1', language: 'c')
	add_global_arguments('-DRASSERT_ENABLE=1', language: 'c')
endif

add_global_link_arguments('-lrt', language: 'c') 

executable('fuck-monitor', src)

