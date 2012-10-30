from gi.repository import GObject, Gio
from ..overrides import override
from ..importer import modules
from gi import types

Ggit = modules['Ggit']._introspection_module
__all__ = []

def _wrap_to_string(self):
    b = type(self).__base__
    mod = b.__module__

    prefix = "gi.repository."

    if mod.find(prefix) == 0:
        mod = mod[len(prefix):]

    return "<%s.%s at %s: %s>" % (mod, b.__name__, hex(id(self)), self.to_string())

def _wrap_iter(self):
    for i in range(0, self.size()):
        yield self.get(i)

def _wrap_initable_init(self, *args, **kwargs):
    super(GObject.Object, self).__init__(*args, **kwargs)
    Gio.Initable.init(self, None)

def _override_dyn(base, **kwargs):
    nm = base.__name__

    if nm in globals():
        for k in kwargs:
            setattr(globals()[nm], k, kwargs[k])
    else:
        cls = type(nm, (base,), kwargs)
        globals()[nm] = cls
        __all__.append(nm)

for c in dir(Ggit):
    try:
        o = getattr(Ggit, c)
    except:
        pass

    if not isinstance(o, types.GObjectMeta) and not isinstance(o, types.StructMeta):
        continue

    # add __str__ mapping to to_string
    if hasattr(o, 'to_string'):
        _override_dyn(o, __str__=_wrap_to_string)

    # add iterator pattern for objects having _get and _size
    if hasattr(o, 'get') and hasattr(o, 'size'):
        _override_dyn(o, __iter__=_wrap_iter)

    if o.__gtype__.is_a(Gio.Initable):
        _override_dyn(o, __init__=_wrap_initable_init)

def _revision_walker_init(self, repository):
    Ggit.RevisionWalker.__init__(self, repository=repository)
    Gio.Initable.init(self, None)

_override_dyn(Ggit.RevisionWalker, __init__=_revision_walker_init)

# vi:ex:ts=4:et
