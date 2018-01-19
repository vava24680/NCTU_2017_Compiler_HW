; Program name : TestCase/test-global1.p
.class public TestCase/test-global1.p
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
.field public static a I
.field public static b I
.field public static c F
.field public static ss I
.method public static ss()I
.limit stack 40
.limit locals 40
getstatic TestCase/test-global1.p/a I
getstatic TestCase/test-global1.p/b I
iadd
putstatic TestCase/test-global1.p/ss F
getstatic TestCase/test-global1.p/ss I
ireturn
.end method
bipush 3
putstatic TestCase/test-global1.p/a I
bipush 4
putstatic TestCase/test-global1.p/b I
invokestatic ss()I
putstatic TestCase/test-global1.p/b I
