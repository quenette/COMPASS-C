#include <Compass.h>

#include <stdio.h>

/* Test the case where an advice is to an object's pointcut... ensure AST is ok */

typedef void (TestPrototype) (void);
typedef struct {
	int foo;
} TestClass;

void test1(void)
{
	TestClass *pc_obj = (TestClass *) pointcut_ptr();
	int i;

	for (i = 0; i < 3; i++) {
		/* There's no need for "result" to be mangled/made-unique */
		/* "result" should be 10 * [0..2] = [0..20] */
		int result = pc_obj->foo * i;

		/* __func__ should still result in "test2", not "main" once weaved */
		printf("In function \"%s\", result = %d\n", __func__, result);
	}
}

void test2(void)
{
	TestClass *pc_obj = (TestClass *) pointcut_ptr();
	int i;

	for (i = 0; i < 3; i++) {
		/* There's no need for "result" to be mangled/made-unique */
		/* "result" should be 10 * [0..20] = [0..200] */
		int result = pc_obj->foo * i * 10;

		/* __func__ should still result in "test2", not "main" once weaved */
		printf("In function \"%s\", result = %d\n", __func__, result);
	}
}

void body1();

void body2();

int main(int argc, char *argv[])
{
	TestClass *obj1 = malloc(sizeof(TestClass));
	TestClass *obj2 = malloc(sizeof(TestClass));

	coInit(&argc, &argv, coStaticWeave | coStaticAdvice);

	advice_object(body1, TestPrototype, obj1, test1, coAdviceAppend);
	advice_object(body1, TestPrototype, obj1, test2, coAdviceAppend);

	advice_object(body2, TestPrototype, obj2, test1, coAdviceAppend);

	coWeave();

	body1(obj1);
	body2(obj2);

	coFinalise();
	return 0;
}

void body1(TestClass * obj)
{
	obj->foo = 10;
	pointcut_object(TestPrototype, obj);
}

void body2(TestClass * obj)
{
	obj->foo = 5;
	pointcut_object(TestPrototype, obj);
}
