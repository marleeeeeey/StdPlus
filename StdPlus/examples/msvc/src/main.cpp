#include "StdPlus/StdPlus.h"

void example_convert_plus()
{
    LOG_STD_FUN;

    short short_value = 0xF0F0;
    std::string short_str = stdplus::to_string_hex(short_value);
    LOG_STD_VAR(short_str);

    int int_value = 0xF0F0;
    std::string int_str = stdplus::to_string_hex(int_value);
    LOG_STD_VAR(int_str);

    LOG_STD_VAR(stdplus::to_string(10 + 20));

    bool bool_value = false;
    LOG_STD_VAR(bool_value);

    std::string str_value = "345";
    int val = stdplus::to<int>(str_value);
    LOG_STD_VAR(val);

    try
    {
        std::string str_value_wrong = "asd345";
        LOG_STD_VAR(str_value_wrong);
        int val_wrong = stdplus::to<int>(str_value_wrong);
        LOG_STD_VAR(val_wrong);
    }
    catch (const std::exception& ex_wrong_value)
    {
        LOG_STD_VAR(ex_wrong_value.what());
    }

    LOG_STD_SPLIT;
}

void example_container_plus()
{
    LOG_STD_FUN;

    std::vector<int> int_vector{ 234, 23, 7, 42, 234, 12, 89 };
    LOG_STD_VAR(int_vector);
    auto double_vector = stdplus::convert<int, double>(int_vector);
    LOG_STD_VAR(double_vector);

    size_t s = 10;
    double * double_c_arr = new double[s];
    for (int i = 0; i < s; ++i)
    {
        double_c_arr[i] = stdplus::getRandom<double>(10, 20);
        std::cout << i << "=" << double_c_arr[i] << std::endl;
    }

    auto double_vector2 = stdplus::to_vector<double>(double_c_arr, s);
    LOG_STD_VAR(double_vector2);

    LOG_STD_SPLIT;
}

int main()
{
    std::ofstream fs("output.txt");
    LOG_STD_REGISTER_STREAM(fs);

    //example_convert_plus();
    example_container_plus();

	stdplus::Time t = stdplus::getCurrentTime();
	std::cout << t << std::endl;
	
	
    LOG_STD_PAUSE;
}