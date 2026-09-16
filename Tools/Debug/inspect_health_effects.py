import unreal
for name in ['GE_J_Trap', 'GE_J_Health_Restoration', 'GE_J_BlockHealthRegen']:
    cls = unreal.load_class(None, '/Jelly/Character/Ability/' + name + '.' + name + '_C')
    if not cls:
        unreal.log_warning('HPDEBUG missing ' + name)
        continue
    obj = unreal.get_default_object(cls)
    unreal.log('HPDEBUG ' + name)
    for prop in ['duration_policy', 'duration_magnitude', 'period', 'periodic_inhibition_policy', 'modifiers', 'ge_components']:
        try:
            unreal.log('HPDEBUG ' + prop + ': ' + str(obj.get_editor_property(prop)))
        except Exception as e:
            unreal.log('HPDEBUG ' + str(e))
    for mod in obj.get_editor_property('modifiers'):
        for prop in ['attribute', 'modifier_op', 'modifier_magnitude']:
            unreal.log('HPDEBUG modifier ' + prop + ': ' + str(mod.get_editor_property(prop)))
        mag = mod.get_editor_property('modifier_magnitude')
        for prop in ['magnitude_calculation_type', 'scalable_float_magnitude']:
            unreal.log('HPDEBUG magnitude ' + prop + ': ' + str(mag.get_editor_property(prop)))
