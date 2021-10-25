#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include "argh.h"

const std::string REQUIRE_PASSWORD_COMMAND = "sudo echo 'PASSWORD IS CORRECT'";
const std::string SHUTDOWN_COMMAND = "sudo shutdown -h +";
const std::string KILLALL_SHUTDOWNS_AND_SAFARI_COMMAND = "sudo killall shutdown & sudo killall Safari";
const std::string KILLALL_SHUTDOWNS_COMMAND = "sudo killall shutdown";
const std::string SET_VOLUME_COMMAND = "sudo osascript ./scripts/vol.applescript";
const std::string MAXIMIZE_SAFARI_COMMAND = "sudo sleep 3 && osascript ./scripts/ccf.applescript";

const std::vector<std::string>
    valid_param_names = {"h", "hour", "m", "minute", "stop"};

const std::string VIDEO_URL = "https://www.youtube.com/embed/vqXNkEc-WaA?start=1&fs=1&autoplay=1";

bool check_for_invalid_parameters(argh::parser &cmdl);
int calculate_minutes(int, int);

int main(int argc, char const *argv[])
{

    argh::parser cmdl(argv, argh::parser::Mode::PREFER_PARAM_FOR_UNREG_OPTION);

    if (!check_for_invalid_parameters(cmdl))
        return -1;

    int hour;
    int minute;

    if (cmdl["stop"])
    {
        return system(KILLALL_SHUTDOWNS_AND_SAFARI_COMMAND.c_str()) == 0 ? 0 : -1;
    }

    (cmdl({"-h", "--hour"}, 0) >> hour);

    if (hour < 0)
    {
        std::cerr << "Invalid parameter. <hour> can not be smaller than 0" << std::endl;
        return -1;
    }

    if (!(cmdl({"-m", "--minute"}) >> minute))
    {
        if (hour == 0)
        {

            std::cerr
                << "Must provide a valid minute value!" << std::endl;
            std::cerr
                << "\nRun 'relax_mode -h <hour> -m <minute>' or 'relax_mode -m <minute>'" << std::endl;
            return -1;
        }
        else
        {
            minute = 0;
        }
    }

    if (minute < 0)
    {
        std::cerr << "Invalid parameter. <minute> can not be smaller than 0" << std::endl;
        return -1;
    }

    if (minute > 59)
    {
        std::cerr << "Invalid parameter. <minute> can not be bigger than 59" << std::endl;
        return -1;
    }

    if (hour == 0 && minute == 0)
    {
        std::cerr << "both <hour> and <minute> can not be equal to 0" << std::endl;
        return -1;
    }

    int minutes = calculate_minutes(hour, minute);

    if (minutes > 300)
    {
        std::cerr << "Maximum time allowed is 6 hours (300 minutes). You entered " << minutes << " minutes" << std::endl;
        return -1;
    }
    else
    {
        system(KILLALL_SHUTDOWNS_COMMAND.c_str());
        int required_password_rv = system(REQUIRE_PASSWORD_COMMAND.c_str());

        if (required_password_rv == 0)
        {

            std::string shutdown_command = (SHUTDOWN_COMMAND + std::to_string(minutes)).c_str();

            std::string open_safari_with_url_command = "open -a Safari '" + VIDEO_URL + "'";

            std::string full_command = open_safari_with_url_command + " && " + MAXIMIZE_SAFARI_COMMAND + " && " + shutdown_command + " && " + SET_VOLUME_COMMAND;

            int shutdown_rv = system(full_command.c_str());

            if (shutdown_rv == 0)
            {
                std::cout << "System will shutdown after " << hour << "hour " << minute << "minutes" << std::endl;
            }
            else
            {
                std::cerr << "shutdown command is not successfull (result: " << shutdown_rv << ")\n";
            }
        }
        else
        {

            std::cerr << "password is not correct (result: " << required_password_rv << ")\n";
        }
    }

    return 0;
}

bool check_for_invalid_parameters(argh::parser &cmdl)
{
    for (auto &param : cmdl.params())
    {
        if (std::find(valid_param_names.begin(), valid_param_names.end(), param.first) != valid_param_names.end())
        {
            continue;
        }
        else
        {
            std::cout << param.first << " is not a valid parameter type!\nRun 'relax_mode -h <hour> -m <minute>'" << std::endl;
            return false;
        }
    }

    return true;
}

int calculate_minutes(int h, int m)
{
    return 60 * h + m;
}