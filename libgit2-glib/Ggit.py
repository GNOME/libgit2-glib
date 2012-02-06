from gi.repository import GObject
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

for c in dir(Ggit):
    try:
        o = getattr(Ggit, c)
    except:
        pass

    if not isinstance(o, types.GObjectMeta) and not isinstance(o, types.StructMeta):
        continue

    if hasattr(o, 'to_string'):
        nm = o.__name__

        cls = type(nm, (o,), {'__str__': _wrap_to_string})
        globals()[nm] = override(cls)
        __all__.append(nm)

class RevisionWalker(Ggit.RevisionWalker):
    def __init__(self, repository):
        Ggit.RevisionWalker.__init__(self, repository=repository)
        self.init(None)

RevisionWalker = override(RevisionWalker)
__all__.append('RevisionWalker')

# vi:ex:ts=4:et
