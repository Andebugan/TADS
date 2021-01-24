int test_matrix(void);
int test_matrix_io(void);

int main(void)
{
    int fails = test_matrix();
    fails += test_matrix_io();

    return fails;
}
