int test_matrix(void);
int test_matrix_io(void);
int test_zip(void);

int main(void)
{
    int fails = 0;

    fails += test_matrix();
    fails += test_matrix_io();
    fails += test_zip();

    return fails;
}
