void makeAnalysis()
{
    // create objects to work with files
    std::ifstream slab_1_population("fl1Current.txt");
    std::ifstream slab_2_population("fl2Current.txt");
    //std::ifstream lg_1_population("fl1LgCurrent.txt");
    //std::ifstream lg_2_population("fl2LgCurrent.txt");


    // two loops for filling out a vector of ifstream objects associated with
    // the files created by the scorers positioned along the slab
    std::vector<std::shared_ptr<std::ifstream>> myFiles;
    for (int i = -60; i < 0; i += 10)
    {
        std::ostringstream filename;
        filename << "popul_at_" << i << "_cm.txt";
        myFiles.push_back( std::make_shared<std::ifstream>( filename.str() ) );
    }
    for (int i = 10; i <= 60; i += 10)
    {
        std::ostringstream filename;
        filename << "popul_at_" << i << "_cm.txt";
        myFiles.push_back( std::make_shared<std::ifstream>( filename.str() ) );
    }


    // the lists with the lines in the files created by the scorers
    std::string list_slab1[5] = {};
    std::string list_slab2[5] = {};
    //std::string list_lg1[5] = {};
    //std::string list_lg2[5] = {};

    std::string list_slab[12][5] = {};

    // counters for filling out the lists
    int count_slab1 = 0;
    int count_slab2 = 0;
    //int count_lg1 = 0;
    //int count_lg2 = 0;

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

/*    // read the file fl1LgCurrent.txt
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
*/

    // read the files created by the scorers positioned along the slab
    for (int i = 0; i < 12; ++i)
    {
        int j = 0;
        while (*myFiles[i])
        {
            getline(*myFiles[i], list_slab[i][j]);
            std::cout << list_slab[i][j] << std::endl;
            ++j;
        }
    }


    // lists with the lines of numbers (4th line in each file) in the files created by the scorers
    //std::string list_of_strings[4] = {list_slab1[3], list_slab2[3], list_lg1[3], list_lg2[3]};
    std::string list_of_strings[2] = {list_slab1[3], list_slab2[3]};

    std::string list_of_strings_along_slab[12] = {};
    for (int i = 0; i < 12; ++i)
        list_of_strings_along_slab[i] = list_slab[i][3];



    // the numbers of particles counted with the scorers
    //int list_of_numbers[4] = {};
    int list_of_numbers[2] = {};
    int list_of_numbers_along_slab[12] = {};


    // it will be used for getting the number we need from the line
    std::string delimiter = ",";

    // a loop over list_of_strings[4] in order to fill out the list_of_numbers[4]
    for (int i = 0; i < 2; ++i)
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


    // a loop over list_of_strings_along_slab[12] in order to fill out the list_of_numbers_along_slab[12]
    for (int i = 0; i < 12; ++i)
    {
        size_t position = 0;
        int count = 0;

        std::string token = "";
        std::string output_string = "";

        // a loop over the given line in list_of_strings_along_slab[12]
        while ((position = list_of_strings_along_slab[i].find(delimiter)) != std::string::npos)
        {

            token = list_of_strings_along_slab[i].substr(0, position);
            if (count == 3)
                output_string = token;

            list_of_strings_along_slab[i].erase(0, position + delimiter.length());
            ++count;
        }

        list_of_numbers_along_slab[i] = std::stoi(output_string);
    }


    // a canvas with the particles counted before and after light guides
    TCanvas *counts_slab_lg_canvas = new TCanvas("counts_slab_lg_canvas", "counts_slab_lg_canvas");
    counts_slab_lg_canvas->cd();

    // output histogram with the particles counted before and after light guides
    TH1I *counts_slab_lg = new TH1I("counts_slab_lg", " ", 12, 0.0, 12.0);

    // setting up the content of the histogram
    //counts_slab_lg->SetBinContent(2,list_of_numbers[2]);
    counts_slab_lg->SetBinContent(4,list_of_numbers[0]);
    counts_slab_lg->SetBinContent(9,list_of_numbers[1]);
    //counts_slab_lg->SetBinContent(11,list_of_numbers[3]);

    // drawing the histogram on the canvas (counts_slab_lg)
    counts_slab_lg->Draw();


    ////// FROM -70 CM TO -10 CM //////

    // a canvas with the particles counted at range(-70, -10, 10)
    TCanvas *att_len_minus = new TCanvas("att_len_minus", "att_len_minus");
    att_len_minus->cd();

    // the histogram for attenuation length measurements (from -70 cm to -10 cm)
    TH1I *attenuation_length_minus_x = new TH1I("attenuation_length_minus_x", " ", 7, -75., -5.);

    // filling out the histogram
    attenuation_length_minus_x->SetBinContent(1, list_of_numbers[0]);
    for (int i = 1; i < 7; ++i)
        attenuation_length_minus_x->SetBinContent(i + 1, list_of_numbers_along_slab[i - 1]);

    // drawing the histogram on the canvas (att_len_minus)
    attenuation_length_minus_x->Draw();

    // the function for fitting the histogram
    TF1 *expo_minus = new TF1("expo_minus","[0]*exp(x/[1])", -75., -5.);
    expo_minus->SetParNames("ampl", "att_len");
    expo_minus->SetParameter(0, 20000);
    expo_minus->SetParameter(1, 400);
    expo_minus->SetLineColor(kRed);

    // fitting the histogram
    attenuation_length_minus_x->Fit("expo_minus");

    // an object for adding a value of attenuation length on the canvas (att_len_minus)
    std::ostringstream tmp;

    // adding a value of attenuation length to the canvas (att_len_minus)
    tmp << "#lambda_{minus} = " << round(expo_minus->GetParameter(1) * 10.) / 10. << " cm";
    TLatex lambda1 = TLatex();
    lambda1.SetTextSize(0.045);
    lambda1.SetNDC();
    lambda1.DrawLatex(0.4, 0.7, tmp.str().c_str());
    tmp.str("");

    ////// FROM 10 CM TO 70 CM //////

    // a canvas with the particles counted at range(10, 70, 10)
    TCanvas *att_len_plus = new TCanvas("att_len_plus", "att_len_plus");
    att_len_plus->cd();

    // the histogram for attenuation length measurements (from 10 cm to 70 cm)
    TH1I *attenuation_length_plus_x = new TH1I("attenuation_length_plus_x", " ", 7, 5., 75.);

    // filling out the histogram
    attenuation_length_plus_x->SetBinContent(7, list_of_numbers[1]);
    for (int i = 1; i < 7; ++i)
        attenuation_length_plus_x->SetBinContent(i, list_of_numbers_along_slab[i + 5]);

    // drawing the histogram on the canvas (att_len_plus)
    attenuation_length_plus_x->Draw();

    // the function for fitting the histogram
    TF1 *expo_plus = new TF1("expo_plus","[0]*exp(-x/[1])", 5., 75.);
    expo_plus->SetParNames("ampl", "att_len");
    expo_plus->SetParameter(0, 20000);
    expo_plus->SetParameter(1, 400);
    expo_plus->SetLineColor(kRed);

    // fitting the histogram
    attenuation_length_plus_x->Fit("expo_plus");

    // adding a value of attenuation length to the canvas (att_len_plus)
    tmp << "#lambda_{plus} = " << round(expo_plus->GetParameter(1) * 10.) / 10. << " cm";
    TLatex lambda2 = TLatex();
    lambda2.SetTextSize(0.045);
    lambda2.SetNDC();
    lambda2.DrawLatex(0.4, 0.7, tmp.str().c_str());
    tmp.str("");


    // output root file
    TFile *file = new TFile("histo.root", "RECREATE");

    counts_slab_lg_canvas->Write();
    att_len_minus->Write();
    att_len_plus->Write();

    file->Close();

}
