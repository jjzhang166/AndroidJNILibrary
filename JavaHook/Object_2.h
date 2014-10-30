/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Declaration of the fundamental Object type and refinements thereof, plus
 * some functions for manipulating them.
 */
#ifndef _DALVIK_OO_OBJECT
#define _DALVIK_OO_OBJECT

#include <stddef.h>
#include <stdbool.h>
#include <jni.h>

// #define ANDROID_VERSION_22 1

/* fwd decl */
struct DataObject;
struct InitiatingLoaderList;
struct ClassObject;
struct StringObject;
struct ArrayObject;
struct Method;
struct ExceptionEntry;
struct LineNumEntry;
struct StaticField;
struct InstField;
struct Field;
struct RegisterMap;
struct DvmDex;
struct DexFile;
struct Thread;

typedef struct DataObject DataObject;
typedef struct InitiatingLoaderList InitiatingLoaderList;
typedef struct ClassObject ClassObject;
typedef struct StringObject StringObject;
typedef struct ArrayObject ArrayObject;
typedef struct Method Method;
typedef struct ExceptionEntry ExceptionEntry;
typedef struct LineNumEntry LineNumEntry;
typedef struct StaticField StaticField;
typedef struct InstField InstField;
typedef struct Field Field;
typedef struct RegisterMap RegisterMap;
typedef struct DvmDex DvmDex;
typedef struct DexFile DexFile;

#ifdef HAVE_STDINT_H
# include <stdint.h>    /* C99 */
typedef uint8_t             u1;
typedef uint16_t            u2;
typedef uint32_t            u4;
typedef uint64_t            u8;
typedef int8_t              s1;
typedef int16_t             s2;
typedef int32_t             s4;
typedef int64_t             s8;
#else
typedef unsigned char       u1;
typedef unsigned short      u2;
typedef unsigned int        u4;
typedef unsigned long long  u8;
typedef signed char         s1;
typedef signed short        s2;
typedef signed int          s4;
typedef signed long long    s8;
#endif

# define STRING_FIELDOFF_VALUE      8
# define STRING_FIELDOFF_HASHCODE   12
# define STRING_FIELDOFF_OFFSET     16
# define STRING_FIELDOFF_COUNT      20

typedef union JValue {
    u1      z;
    s1      b;
    u2      c;
    s2      s;
    s4      i;
    s8      j;
    float   f;
    double  d;
    void*   l;
} JValue;

typedef struct DexProto {
    const DexFile* dexFile;     /* file the idx refers to */
    u4 protoIdx;                /* index into proto_ids table of dexFile */
} DexProto;

/*
 * Native function pointer type.
 *
 * "args[0]" holds the "this" pointer for virtual methods.
 *
 * The "Bridge" form is a super-set of the "Native" form; in many places
 * they are used interchangeably.  Currently, all functions have all
 * arguments passed in, but some functions only care about the first two.
 * Passing extra arguments to a C function is (mostly) harmless.
 */
typedef void (*DalvikBridgeFunc)(const u4* args, JValue* pResult,
    const Method* method, void* self);
typedef void (*DalvikNativeFunc)(const u4* args, JValue* pResult);

/*
 * access flags and masks; the "standard" ones are all <= 0x4000
 * 
 * Note: There are related declarations in vm/oo/Object.h in the ClassFlags
 * enum.
 */
enum {
    ACC_PUBLIC       = 0x00000001,       // class, field, method, ic
    ACC_PRIVATE      = 0x00000002,       // field, method, ic
    ACC_PROTECTED    = 0x00000004,       // field, method, ic
    ACC_STATIC       = 0x00000008,       // field, method, ic
    ACC_FINAL        = 0x00000010,       // class, field, method, ic
    ACC_SYNCHRONIZED = 0x00000020,       // method (only allowed on natives)
    ACC_SUPER        = 0x00000020,       // class (not used in Dalvik)
    ACC_VOLATILE     = 0x00000040,       // field
    ACC_BRIDGE       = 0x00000040,       // method (1.5)
    ACC_TRANSIENT    = 0x00000080,       // field
    ACC_VARARGS      = 0x00000080,       // method (1.5)
    ACC_NATIVE       = 0x00000100,       // method
    ACC_INTERFACE    = 0x00000200,       // class, ic
    ACC_ABSTRACT     = 0x00000400,       // class, method, ic
    ACC_STRICT       = 0x00000800,       // method
    ACC_SYNTHETIC    = 0x00001000,       // field, method, ic
    ACC_ANNOTATION   = 0x00002000,       // class, ic (1.5)
    ACC_ENUM         = 0x00004000,       // class, field, ic (1.5)
    ACC_CONSTRUCTOR  = 0x00010000,       // method (Dalvik only)
    ACC_DECLARED_SYNCHRONIZED =
                       0x00020000,       // method (Dalvik only)
    ACC_CLASS_MASK = 
        (ACC_PUBLIC | ACC_FINAL | ACC_INTERFACE | ACC_ABSTRACT
                | ACC_SYNTHETIC | ACC_ANNOTATION | ACC_ENUM),
    ACC_INNER_CLASS_MASK =
        (ACC_CLASS_MASK | ACC_PRIVATE | ACC_PROTECTED | ACC_STATIC),
    ACC_FIELD_MASK =
        (ACC_PUBLIC | ACC_PRIVATE | ACC_PROTECTED | ACC_STATIC | ACC_FINAL
                | ACC_VOLATILE | ACC_TRANSIENT | ACC_SYNTHETIC | ACC_ENUM),
    ACC_METHOD_MASK =
        (ACC_PUBLIC | ACC_PRIVATE | ACC_PROTECTED | ACC_STATIC | ACC_FINAL
                | ACC_SYNCHRONIZED | ACC_BRIDGE | ACC_VARARGS | ACC_NATIVE
                | ACC_ABSTRACT | ACC_STRICT | ACC_SYNTHETIC | ACC_CONSTRUCTOR
                | ACC_DECLARED_SYNCHRONIZED),
};



/* vm-internal access flags and related definitions */
typedef enum AccessFlags {
    ACC_MIRANDA         = 0x8000,       // method (internal to VM)
    JAVA_FLAGS_MASK     = 0xffff,       // bits set from Java sources (low 16)
} AccessFlags;

/* Use the top 16 bits of the access flags field for
 * other class flags.  Code should use the *CLASS_FLAG*()
 * macros to set/get these flags.
 */
typedef enum ClassFlags {
    CLASS_ISFINALIZABLE     = (1<<31),  // class/ancestor overrides finalize()
    CLASS_ISARRAY           = (1<<30),  // class is a "[*"
    CLASS_ISOBJECTARRAY     = (1<<29),  // class is a "[L*" or "[[*"
    CLASS_ISREFERENCE       = (1<<28),  // class is a soft/weak/phantom ref
                                        // only ISREFERENCE is set --> soft
    CLASS_ISWEAKREFERENCE   = (1<<27),  // class is a weak reference
    CLASS_ISPHANTOMREFERENCE = (1<<26), // class is a phantom reference

    CLASS_MULTIPLE_DEFS     = (1<<25),  // DEX verifier: defs in multiple DEXs

    /* unlike the others, these can be present in the optimized DEX file */
    CLASS_ISOPTIMIZED       = (1<<17),  // class may contain opt instrs
    CLASS_ISPREVERIFIED     = (1<<16),  // class has been pre-verified
} ClassFlags;

/* bits we can reasonably expect to see set in a DEX access flags field */
#define EXPECTED_FILE_FLAGS \
    (ACC_CLASS_MASK | CLASS_ISPREVERIFIED | CLASS_ISOPTIMIZED)

/*
 * Get/set class flags.
 */
#define SET_CLASS_FLAG(clazz, flag) \
    do { (clazz)->accessFlags |= (flag); } while (0)

#define CLEAR_CLASS_FLAG(clazz, flag) \
    do { (clazz)->accessFlags &= ~(flag); } while (0)

#define IS_CLASS_FLAG_SET(clazz, flag) \
    (((clazz)->accessFlags & (flag)) != 0)

#define GET_CLASS_FLAG_GROUP(clazz, flags) \
    ((u4)((clazz)->accessFlags & (flags)))


#define RETURN_VOID()           do { pResult->i = 0xfefeabab; return; }while(0)
#define RETURN_BOOLEAN(_val)    do { pResult->i = (_val); return; } while(0)
#define RETURN_INT(_val)        do { pResult->i = (_val); return; } while(0)
#define RETURN_LONG(_val)       do { pResult->j = (_val); return; } while(0)
#define RETURN_FLOAT(_val)      do { pResult->f = (_val); return; } while(0)
#define RETURN_DOUBLE(_val)     do { pResult->d = (_val); return; } while(0)
#define RETURN_PTR(_val)        do { pResult->l = (_val); return; } while(0)

#ifndef INLINE
# define INLINE extern __inline__
#endif
/*
 * Use the top 16 bits of the access flags field for other method flags.
 * Code should use the *METHOD_FLAG*() macros to set/get these flags.
 */
typedef enum MethodFlags {
    METHOD_ISWRITABLE       = (1<<31),  // the method's code is writable
} MethodFlags;

/*
 * Get/set method flags.
 */
#define SET_METHOD_FLAG(method, flag) \
    do { (method)->accessFlags |= (flag); } while (0)

#define CLEAR_METHOD_FLAG(method, flag) \
    do { (method)->accessFlags &= ~(flag); } while (0)

#define IS_METHOD_FLAG_SET(method, flag) \
    (((method)->accessFlags & (flag)) != 0)

#define GET_METHOD_FLAG_GROUP(method, flags) \
    ((u4)((method)->accessFlags & (flags)))

/* current state of the class, increasing as we progress */
typedef enum ClassStatus {
    CLASS_ERROR         = -1,

    CLASS_NOTREADY      = 0,
    CLASS_LOADED        = 1,
    CLASS_PREPARED      = 2,    /* part of linking */
    CLASS_RESOLVED      = 3,    /* part of linking */
    CLASS_VERIFYING     = 4,    /* in the process of being verified */
    CLASS_VERIFIED      = 5,    /* logically part of linking; done pre-init */
    CLASS_INITIALIZING  = 6,    /* class init in progress */
    CLASS_INITIALIZED   = 7,    /* ready to go */
} ClassStatus;


/*
 * Primitive type identifiers.  We use these values as indexes into an
 * array of synthesized classes, so these start at zero and count up.
 * The order is arbitrary (mimics table in doc for newarray opcode),
 * but can't be changed without shuffling some reflection tables.
 *
 * PRIM_VOID can't be used as an array type, but we include it here for
 * other uses (e.g. Void.TYPE).
 */
typedef enum PrimitiveType {
    PRIM_NOT        = -1,       /* value is not a primitive type */
    PRIM_BOOLEAN    = 0,
    PRIM_CHAR       = 1,
    PRIM_FLOAT      = 2,
    PRIM_DOUBLE     = 3,
    PRIM_BYTE       = 4,
    PRIM_SHORT      = 5,
    PRIM_INT        = 6,
    PRIM_LONG       = 7,
    PRIM_VOID       = 8,

    PRIM_MAX
} PrimitiveType;
#define PRIM_TYPE_TO_LETTER "ZCFDBSIJV"     /* must match order in enum */

/*
 * Definitions for packing refOffsets in ClassObject.
 */
/*
 * A magic value for refOffsets. Ignore the bits and walk the super
 * chain when this is the value.
 * [This is an unlikely "natural" value, since it would be 30 non-ref instance
 * fields followed by 2 ref instance fields.]
 */
#define CLASS_WALK_SUPER ((unsigned int)(3))
#define CLASS_SMALLEST_OFFSET (sizeof(struct Object))
#define CLASS_BITS_PER_WORD (sizeof(unsigned long int) * 8)
#define CLASS_OFFSET_ALIGNMENT 4
#define CLASS_HIGH_BIT ((unsigned int)1 << (CLASS_BITS_PER_WORD - 1))
/*
 * Given an offset, return the bit number which would encode that offset.
 * Local use only.
 */
#define _CLASS_BIT_NUMBER_FROM_OFFSET(byteOffset) \
    (((unsigned int)(byteOffset) - CLASS_SMALLEST_OFFSET) / \
     CLASS_OFFSET_ALIGNMENT)
/*
 * Is the given offset too large to be encoded?
 */
#define CLASS_CAN_ENCODE_OFFSET(byteOffset) \
    (_CLASS_BIT_NUMBER_FROM_OFFSET(byteOffset) < CLASS_BITS_PER_WORD)
/*
 * Return a single bit, encoding the offset.
 * Undefined if the offset is too large, as defined above.
 */
#define CLASS_BIT_FROM_OFFSET(byteOffset) \
    (CLASS_HIGH_BIT >> _CLASS_BIT_NUMBER_FROM_OFFSET(byteOffset))
/*
 * Return an offset, given a bit number as returned from CLZ.
 */
#define CLASS_OFFSET_FROM_CLZ(rshift) \
    (((int)(rshift) * CLASS_OFFSET_ALIGNMENT) + CLASS_SMALLEST_OFFSET)


/*
 * Used for iftable in ClassObject.
 */
typedef struct InterfaceEntry {
    /* pointer to interface class */
    ClassObject*    clazz;

    /*
     * Index into array of vtable offsets.  This points into the ifviPool,
     * which holds the vtables for all interfaces declared by this class.
     */
    int*            methodIndexArray;
} InterfaceEntry;



/*
 * There are three types of objects:
 *  Class objects - an instance of java.lang.Class
 *  Array objects - an object created with a "new array" instruction
 *  Data objects - an object that is neither of the above
 *
 * We also define String objects.  At present they're equivalent to
 * DataObject, but that may change.  (Either way, they make some of the
 * code more obvious.)
 *
 * All objects have an Object header followed by type-specific data.
 */
typedef struct Object {
    /* ptr to class object */
    ClassObject*    clazz;

    /*
     * A word containing either a "thin" lock or a "fat" monitor.  See
     * the comments in Sync.c for a description of its layout.
     */
    u4              lock;
} Object;

/*
 * Properly initialize an Object.
 * void DVM_OBJECT_INIT(Object *obj, ClassObject *clazz_)
 */
#define DVM_OBJECT_INIT(obj, clazz_) \
    do { (obj)->clazz = (clazz_); DVM_LOCK_INIT(&(obj)->lock); } while (0)

/*
 * Data objects have an Object header followed by their instance data.
 */
struct DataObject {
    Object          obj;                /* MUST be first item */

    /* variable #of u4 slots; u8 uses 2 slots */
    u4              instanceData[1];
};

/*
 * Strings are used frequently enough that we may want to give them their
 * own unique type.
 *
 * Using a dedicated type object to access the instance data provides a
 * performance advantage but makes the java/lang/String.java implementation
 * fragile.
 *
 * Currently this is just equal to DataObject, and we pull the fields out
 * like we do for any other object.
 */
struct StringObject {
    Object          obj;                /* MUST be first item */

    /* variable #of u4 slots; u8 uses 2 slots */
    u4              instanceData[1];
};


/*
 * Array objects have these additional fields.
 *
 * We don't currently store the size of each element.  Usually it's implied
 * by the instruction.  If necessary, the width can be derived from
 * the first char of obj->clazz->descriptor.
 */
struct ArrayObject {
    Object          obj;                /* MUST be first item */

    /* number of elements; immutable after init */
    u4              length;

    /*
     * Array contents; actual size is (length * sizeof(type)).  This is
     * declared as u8 so that the compiler inserts any necessary padding
     * (e.g. for EABI); the actual allocation may be smaller than 8 bytes.
     */
    u8              contents[1];
};

/*
 * For classes created early and thus probably in the zygote, the
 * InitiatingLoaderList is kept in gDvm. Later classes use the structure in
 * Object Class. This helps keep zygote pages shared.
 */
struct InitiatingLoaderList {
    /* a list of initiating loader Objects; grown and initialized on demand */
    Object**  initiatingLoaders;
    /* count of loaders in the above list */
    int       initiatingLoaderCount;
};

struct ProxyClassObject {
	void*         env;
	ClassObject*  clazz;
	Method*       orgMeghod;
};

/*
 * This defines the amount of space we leave for field slots in the
 * java.lang.Class definition.  If we alter the class to have more than
 * this many fields, the VM will abort at startup.
 */
#define CLASS_FIELD_SLOTS   4

/*
 * Class objects have many additional fields.  This is used for both
 * classes and interfaces, including synthesized classes (arrays and
 * primitive types).
 *
 * Class objects are unusual in that they have some fields allocated with
 * the system malloc (or LinearAlloc), rather than on the GC heap.  This is
 * handy during initialization, but does require special handling when
 * discarding java.lang.Class objects.
 *
 * The separation of methods (direct vs. virtual) and fields (class vs.
 * instance) used in Dalvik works out pretty well.  The only time it's
 * annoying is when enumerating or searching for things with reflection.
 */
 
struct ClassObject {
    Object          obj;                /* MUST be first item */

    /* leave space for instance data; we could access fields directly if we
       freeze the definition of java/lang/Class */
    u4              instanceData[CLASS_FIELD_SLOTS];

    /* UTF-8 descriptor for the class; from constant pool, or on heap
       if generated ("[C") */
    const char*     descriptor;
    char*           descriptorAlloc;

    /* access flags; low 16 bits are defined by VM spec */
    u4              accessFlags;

    /* VM-unique class serial number, nonzero, set very early */
    u4              serialNumber;

    /* DexFile from which we came; needed to resolve constant pool entries */
    /* (will be NULL for VM-generated, e.g. arrays and primitive classes) */
    DvmDex*         pDvmDex;

    /* state of class initialization */
    ClassStatus     status;

    /* if class verify fails, we must return same error on subsequent tries */
    ClassObject*    verifyErrorClass;

    /* threadId, used to check for recursive <clinit> invocation */
    u4              initThreadId;

    /*
     * Total object size; used when allocating storage on gc heap.  (For
     * interfaces and abstract classes this will be zero.)
     */
    size_t          objectSize;

    /* arrays only: class object for base element, for instanceof/checkcast
       (for String[][][], this will be String) */
    ClassObject*    elementClass;
	
#ifndef ANDROID_VERSION_22
	/* class object representing an array of this class; set on first use */
    ClassObject*    arrayClass;
#endif

    /* arrays only: number of dimensions, e.g. int[][] is 2 */
    int             arrayDim;

    /* primitive type index, or PRIM_NOT (-1); set for generated prim classes */
    PrimitiveType   primitiveType;

    /* superclass, or NULL if this is java.lang.Object */
    ClassObject*    super;

    /* defining class loader, or NULL for the "bootstrap" system loader */
    Object*         classLoader;

    /* initiating class loader list */
    /* NOTE: for classes with low serialNumber, these are unused, and the
       values are kept in a table in gDvm. */
    InitiatingLoaderList initiatingLoaderList;

    /* array of interfaces this class implements directly */
    int             interfaceCount;
    ClassObject**   interfaces;

    /* static, private, and <init> methods */
    int             directMethodCount;
    Method*         directMethods;

    /* virtual methods defined in this class; invoked through vtable */
    int             virtualMethodCount;
    Method*         virtualMethods;

    /*
     * Virtual method table (vtable), for use by "invoke-virtual".  The
     * vtable from the superclass is copied in, and virtual methods from
     * our class either replace those from the super or are appended.
     */
    int             vtableCount;
    Method**        vtable;

    /*
     * Interface table (iftable), one entry per interface supported by
     * this class.  That means one entry for each interface we support
     * directly, indirectly via superclass, or indirectly via
     * superinterface.  This will be null if neither we nor our superclass
     * implement any interfaces.
     *
     * Why we need this: given "class Foo implements Face", declare
     * "Face faceObj = new Foo()".  Invoke faceObj.blah(), where "blah" is
     * part of the Face interface.  We can't easily use a single vtable.
     *
     * For every interface a concrete class implements, we create a list of
     * virtualMethod indices for the methods in the interface.
     */
    int             iftableCount;
    InterfaceEntry* iftable;

    /*
     * The interface vtable indices for iftable get stored here.  By placing
     * them all in a single pool for each class that implements interfaces,
     * we decrease the number of allocations.
     */
    int             ifviPoolCount;
    int*            ifviPool;

    /* static fields */
    int             sfieldCount;
    StaticField*    sfields;

    /* instance fields
     *
     * These describe the layout of the contents of a DataObject-compatible
     * Object.  Note that only the fields directly defined by this class
     * are listed in ifields;  fields defined by a superclass are listed
     * in the superclass's ClassObject.ifields.
     *
     * All instance fields that refer to objects are guaranteed to be
     * at the beginning of the field list.  ifieldRefCount specifies
     * the number of reference fields.
     */
    int             ifieldCount;
    int             ifieldRefCount; // number of fields that are object refs
    InstField*      ifields;

    /* bitmap of offsets of ifields */
    u4 refOffsets;

    /* source file name, if known */
    const char*     sourceFile;
};

/*
 * A method.  We create one of these for every method in every class
 * we load, so try to keep the size to a minimum.
 *
 * Much of this comes from and could be accessed in the data held in shared
 * memory.  We hold it all together here for speed.  Everything but the
 * pointers could be held in a shared table generated by the optimizer;
 * if we're willing to convert them to offsets and take the performance
 * hit (e.g. "meth->insns" becomes "baseAddr + meth->insnsOffset") we
 * could move everything but "nativeFunc".
 */
struct Method {
    /* the class we are a part of */
    ClassObject*    clazz;

    /* access flags; low 16 bits are defined by spec (could be u2?) */
    /*方法的访问标志，可以为ACC_PUBLIC、ACC_PRIVATE、ACC_PROTECTED、ACC_STATIC、ACC_FINAL、ACC_SYNCHRONIZED、ACC_BRIDGE、ACC_VARARGS、ACC_NATIVE、ACC_ABSTRACT、ACC_STRICT、ACC_SYNTHETIC、ACC_CONSTRUCTOR和ACC_DECLARED_SYNCHRONIZED及其组合 */
    u4              accessFlags;

    /*
     * For concrete virtual methods, this is the offset of the method
     * in "vtable".
     *
     * For abstract methods in an interface class, this is the offset
     * of the method in "iftable[n]->methodIndexArray".
     */
    u2             methodIndex;

    /*
     * Method bounds; not needed for an abstract method.
     *
     * For a native method, we compute the size of the argument list, and
     * set "insSize" and "registerSize" equal to it.
     */
    u2              registersSize;  /* ins + locals */
    u2              outsSize;
    u2              insSize;

    /* method name, e.g. "<init>" or "eatLunch" */
    const char*     name;

    /*
     * Method prototype descriptor string (return and argument types).
     *
     * TODO: This currently must specify the DexFile as well as the proto_ids
     * index, because generated Proxy classes don't have a DexFile.  We can
     * remove the DexFile* and reduce the size of this struct if we generate
     * a DEX for proxies.
     */
    DexProto        prototype;

    /* short-form method descriptor string */
    const char*     shorty;

    /*
     * The remaining items are not used for abstract or native methods.
     * (JNI is currently hijacking "insns" as a function pointer, set
     * after the first call.  For internal-native this stays null.)
     */

    /* the actual code */
    const u2*       insns;          /* instructions, in memory-mapped .dex */

    /* cached JNI argument and return-type hints */
    int             jniArgInfo;

    /*
     * Native method ptr; could be actual function or a JNI bridge.  We
     * don't currently discriminate between DalvikBridgeFunc and
     * DalvikNativeFunc; the former takes an argument superset (i.e. two
     * extra args) which will be ignored.  If necessary we can use
     * insns==NULL to detect JNI bridge vs. internal native.
     */
    DalvikBridgeFunc nativeFunc;

    /*
     * Register map data, if available.  This will point into the DEX file
     * if the data was computed during pre-verification, or into the
     * linear alloc area if not.
     */
    const RegisterMap* registerMap;

// #ifdef WITH_PROFILER
    bool            inProfile;
// #endif
};

/*
 * Generic field header.  We pass this around when we want a generic Field
 * pointer (e.g. for reflection stuff).  Testing the accessFlags for
 * ACC_STATIC allows a proper up-cast.
 */
struct Field {
    ClassObject*    clazz;          /* class in which the field is declared */
    const char*     name;
    const char*     signature;      /* e.g. "I", "[C", "Landroid/os/Debug;" */
    u4              accessFlags;
#ifdef PROFILE_FIELD_ACCESS
    u4              gets;
    u4              puts;
#endif
};

/*
 * Static field.
 */
struct StaticField {
    Field           field;          /* MUST be first item */
    JValue          value;          /* initially set from DEX for primitives */
};

/*
 * Instance field.
 */
struct InstField {
    Field           field;          /* MUST be first item */

    /*
     * This field indicates the byte offset from the beginning of the
     * (Object *) to the actual instance data; e.g., byteOffset==0 is
     * the same as the object pointer (bug!), and byteOffset==4 is 4
     * bytes farther.
     */
    int             byteOffset;
};

#define BYTE_OFFSET(_ptr, _offset)  ((void*) (((u1*)(_ptr)) + (_offset)))

INLINE JValue* dvmFieldPtr(const Object* obj, int offset) {
    return ((JValue*)BYTE_OFFSET(obj, offset));
}

INLINE bool dvmGetFieldBoolean(const Object* obj, int offset) {
    return ((JValue*)BYTE_OFFSET(obj, offset))->z;
}
INLINE s1 dvmGetFieldByte(const Object* obj, int offset) {
    return ((JValue*)BYTE_OFFSET(obj, offset))->b;
}
INLINE s2 dvmGetFieldShort(const Object* obj, int offset) {
    return ((JValue*)BYTE_OFFSET(obj, offset))->s;
}
INLINE u2 dvmGetFieldChar(const Object* obj, int offset) {
    return ((JValue*)BYTE_OFFSET(obj, offset))->c;
}
INLINE s4 dvmGetFieldInt(const Object* obj, int offset) {
    return ((JValue*)BYTE_OFFSET(obj, offset))->i;
}

INLINE s8 dvmGetFieldLong(const Object* obj, int offset) {
    return ((JValue*)BYTE_OFFSET(obj, offset))->j;
}
INLINE float dvmGetFieldFloat(const Object* obj, int offset) {
    return ((JValue*)BYTE_OFFSET(obj, offset))->f;
}
INLINE double dvmGetFieldDouble(const Object* obj, int offset) {
    return ((JValue*)BYTE_OFFSET(obj, offset))->d;
}
INLINE Object* dvmGetFieldObject(const Object* obj, int offset) {
    return (Object*)((JValue*)BYTE_OFFSET(obj, offset))->l;
}

INLINE void dvmSetFieldBoolean(Object* obj, int offset, bool val) {
    ((JValue*)BYTE_OFFSET(obj, offset))->i = val;
}
INLINE void dvmSetFieldByte(Object* obj, int offset, s1 val) {
    ((JValue*)BYTE_OFFSET(obj, offset))->i = val;
}
INLINE void dvmSetFieldShort(Object* obj, int offset, s2 val) {
    ((JValue*)BYTE_OFFSET(obj, offset))->i = val;
}
INLINE void dvmSetFieldChar(Object* obj, int offset, u2 val) {
    ((JValue*)BYTE_OFFSET(obj, offset))->i = val;
}
INLINE void dvmSetFieldInt(Object* obj, int offset, s4 val) {
    ((JValue*)BYTE_OFFSET(obj, offset))->i = val;
}
INLINE void dvmSetFieldLong(Object* obj, int offset, s8 val) {
    ((JValue*)BYTE_OFFSET(obj, offset))->j = val;
}
INLINE void dvmSetFieldFloat(Object* obj, int offset, float val) {
    ((JValue*)BYTE_OFFSET(obj, offset))->f = val;
}
INLINE void dvmSetFieldDouble(Object* obj, int offset, double val) {
    ((JValue*)BYTE_OFFSET(obj, offset))->d = val;
}
INLINE void dvmSetFieldObject(Object* obj, int offset, Object* val) {
    ((JValue*)BYTE_OFFSET(obj, offset))->l = val;
}


#endif /*_DALVIK_OO_OBJECT*/
