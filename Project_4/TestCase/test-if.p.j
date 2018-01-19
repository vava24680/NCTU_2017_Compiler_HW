; Program name : TestCase/test-if.p
.class public TestCase/test-if.p
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
.field public static a I
.field public static b I
.field public static aa F
.field public static bb F
.field public static cc F
.field public static c I
.field public static d I
bipush 5
putstatic TestCase/test-if.p/a I
bipush 3
putstatic TestCase/test-if.p/b I
ldc2_w 0
putstatic TestCase/test-if.p/aa F
ldc2_w 0
putstatic TestCase/test-if.p/bb F
ldc2_w 0
putstatic TestCase/test-if.p/cc F
getstatic TestCase/test-if.p/a I
getstatic TestCase/test-if.p/b I
isub
ifge Lfalse0
getstatic TestCase/test-if.p/aa F
getstatic TestCase/test-if.p/bb F
fcmpl
ifge Lfalse1
ldc2_w 0
putstatic TestCase/test-if.p/cc F
goto Lexit1
Lfalse1:
ldc2_w 0
putstatic TestCase/test-if.p/cc F
Lexit1:
goto Lexit0
Lfalse0:
bipush 0
putstatic TestCase/test-if.p/c I
Lexit0:
getstatic TestCase/test-if.p/a I
getstatic TestCase/test-if.p/b I
iadd
putstatic TestCase/test-if.p/d I
