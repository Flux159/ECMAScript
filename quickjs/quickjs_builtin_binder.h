#ifndef QUICKJS_BUILTIN_BINDER_H
#define QUICKJS_BUILTIN_BINDER_H

#include "quickjs/quickjs.h"
#include <core/variant.h>
struct ECMAScriptGCHandler;
class QuickJSBinder;

typedef JSValue (*JSConstructorFunc)(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv);
typedef void (*JSFinalizerFunc)(JSRuntime *rt, JSValue val);

class QuickJSBuiltinBinder {
public:
	struct BuiltinClass {
		JSClassID id;
		JSValue class_function;
		JSValue class_prototype;
		JSClassDef js_class;
	};

private:
	QuickJSBinder *binder;
	JSContext *ctx;
	BuiltinClass *builtin_class_map;
	JSValue to_string_function;
	JSAtom js_key_to_string;

	struct GodotVariantParser {
		Variant::Type type;
		union {
			Transform2D *_transform2d;
			::AABB *_aabb;
			Basis *_basis;
			Transform *_transform;
			void *_ptr; //generic pointer
			uint8_t _mem[sizeof(real_t)];
		} data;
		// } data GCC_ALIGNED_8;
	};

public:
	void builtin_finalizer(ECMAScriptGCHandler *p_bind);
	JSValue bind_builtin_object(Variant::Type p_type, const void *p_object);

	void register_builtin_class(Variant::Type p_type, const char *p_name, JSConstructorFunc p_constructor, int argc);
	void register_property(Variant::Type p_type, const char *p_name, JSCFunctionMagic *p_getter, JSCFunctionMagic *p_setter, int magic);
	void register_operators(Variant::Type p_type, Vector<JSValue> &p_operators);
	void register_method(Variant::Type p_type, const char *p_name, JSCFunction *p_func, int p_length);
	void register_constant(Variant::Type p_type, const char *p_name, const Variant &p_value);
	void get_cross_type_operators(Variant::Type p_type, Vector<JSValue> &r_operators);

public:
	QuickJSBuiltinBinder();
	~QuickJSBuiltinBinder();

	void initialize(JSContext *p_context, QuickJSBinder *p_binder);
	void uninitialize();

	void bind_builtin_classes_gen();
	void bind_builtin_propties_manually();

	_FORCE_INLINE_ void set_classid(Variant::Type p_type, JSClassID p_id) { builtin_class_map[p_type].id = p_id; }
	_FORCE_INLINE_ JSClassID get_classid(Variant::Type p_type) { return builtin_class_map[p_type].id; }
	_FORCE_INLINE_ BuiltinClass &get_class(Variant::Type p_type) { return *(builtin_class_map + p_type); }

	static JSValue bind_builtin_object_static(JSContext *ctx, Variant::Type p_type, const void *p_object);
	static JSValue new_object_from(JSContext *ctx, const Variant &p_val);
	static JSValue new_object_from(JSContext *ctx, const Vector2 &p_val);
	static JSValue new_object_from(JSContext *ctx, const Vector3 &p_val);
	static JSValue new_object_from(JSContext *ctx, const Rect2 &p_val);
	static JSValue new_object_from(JSContext *ctx, const Color &p_val);
	static JSValue new_object_from(JSContext *ctx, const Transform2D &p_val);
	static JSValue new_object_from(JSContext *ctx, const Transform &p_val);
	static JSValue new_object_from(JSContext *ctx, const Quat &p_val);
	static JSValue new_object_from(JSContext *ctx, const Plane &p_val);
	static JSValue new_object_from(JSContext *ctx, const RID &p_val);
	static JSValue new_object_from(JSContext *ctx, const AABB &p_val);
	static JSValue new_object_from(JSContext *ctx, const Basis &p_val);
	static JSValue new_object_from(JSContext *ctx, const PackedInt32Array &p_val);
	static JSValue new_object_from(JSContext *ctx, const PackedByteArray &p_val);
	static JSValue new_object_from(JSContext *ctx, const PackedFloat32Array &p_val);
	static JSValue new_object_from(JSContext *ctx, const PackedColorArray &p_val);
	static JSValue new_object_from(JSContext *ctx, const PackedStringArray &p_val);
	static JSValue new_object_from(JSContext *ctx, const PackedVector2Array &p_val);
	static JSValue new_object_from(JSContext *ctx, const PackedVector3Array &p_val);
};

#endif // QUICKJS_BUILTIN_BINDER_H
