
set(src_intern
	intern/texture-atlas.h
	intern/texture-atlas.c

	intern/texture-font.h
	intern/texture-font.c

	intern/font-manager.h
	intern/font-manager.c

	intern/vertex-buffer.h
	intern/#vertex-buffer.c

	intern/vertex-attribute.h
	intern/#vertex-attribute.c

	intern/distance-field.h
	intern/distance-field.c

	intern/ftgl-utils.h
	intern/ftgl-utils.c

	intern/platform.h
	intern/platform.c

	intern/utf8-utils.h
	intern/utf8-utils.c

	intern/vector.h
	intern/vector.c

	intern/edtaa3func.h
	intern/edtaa3func.c

	intern/vec234.h

	intern/TC_Font.cpp
	intern/TC_FontTexture.cpp
	intern/TC_GlyText.cpp
	intern/TC_GlyChar.cpp

	intern/TC_TextureCache.h
	intern/TC_TextureCache.cpp
)

if(OpenSceneGraph_FOUND)
	list(APPEND src_intern intern/TC_TextNode.cpp)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group("intern" FILES ${src_intern})
