#include "countParticles.hh"

void countParticles()
{
    // create objects to work with files
    std::ifstream slab_1_population("fl1Current.txt");
    std::ifstream slab_2_population("fl2Current.txt");
    std::ifstream lg_1_population("fl1LgCurrent.txt");
    std::ifstream lg_2_population("fl2LgCurrent.txt");

    // if we can't open the files for reading
    if (!slab_1_population || !slab_2_population || !lg_1_population || !lg_2_population)
    {
        std::cerr << "Some file could not be opened for reading!" << std::endl;
        exit(1);
    }

    // the lists with the lines in the files created by scoring commands
    std::string list_slab1[5] = {};
    std::string list_slab2[5] = {};
    std::string list_lg1[5] = {};
    std::string list_lg2[5] = {};

    // counters for filling out the lists
    int count_slab1 = 0;
    int count_slab2 = 0;
    int count_lg1 = 0;
    int count_lg2 = 0;

    std::cout << '\n';

    // read the file fl1Current.txt
    while (slab_1_population)
    {
        getline(slab_1_population, list_slab1[count_slab1]);
        std::cout << list_slab1[count_slab1] << std::endl;
        ++count_slab1;
    }

    // read the file fl2Current.txt
    while (slab_2_population)
    {
        getline(slab_2_population, list_slab2[count_slab2]);
        std::cout << list_slab2[count_slab2] << std::endl;
        ++count_slab2;
    }

    // read the file fl1LgCurrent.txt
    while (lg_1_population)
    {
        getline(lg_1_population, list_lg1[count_lg1]);
        std::cout << list_lg1[count_lg1] << std::endl;
        ++count_lg1;
    }

    // read the file fl2LgCurrent.txt
    while (lg_2_population)
    {
        getline(lg_2_population, list_lg2[count_lg2]);
        std::cout << list_lg2[count_lg2] << std::endl;
        ++count_lg2;
    }

    // list with the lines of numbers in the files created by scoring commands
    std::string list_of_strings[4] = {list_slab1[3], list_slab2[3], list_lg1[3], list_lg2[3]};

    // the numbers of particles countered with the scoring meshes
    int list_of_numbers[4] = {};

    // it will be used for getting the number we need from the line
    std::string delimiter = ",";

    // a loop over list_of_strings[4] in order to fill out the list_of_numbers[4]
    for (int i = 0; i < 4; ++i)
    {
        size_t position = 0;
        int count = 0;

        std::string token = "";
        std::string output_string = "";

        // a loop over the given line in list_of_strings[4]
        while ((position = list_of_strings[i].find(delimiter)) != std::string::npos)
        {

            token = list_of_strings[i].substr(0, position);
            if (count == 3)
                output_string = token;

            list_of_strings[i].erase(0, position + delimiter.length());
            ++count;
        }

        list_of_numbers[i] = std::stoi(output_string);
    }

    // output histogram
    TH1I *out_hist = new TH1I("out_hist", "Numbers of particles", 12, 0.0, 12.0);

    // setting the content of the histogram
    out_hist->SetBinContent(2,list_of_numbers[2]);
    out_hist->SetBinContent(4,list_of_numbers[0]);
    out_hist->SetBinContent(9,list_of_numbers[1]);
    out_hist->SetBinContent(11,list_of_numbers[3]);

    // output root file
    TFile *file = new TFile("histo.root", "RECREATE");

    out_hist->Write();
    file->Close();

}
