void read()
{
    using namespace std;

    ifstream myFile("scoring_begin.mac");

    vector<string> meshesNames;

    while (myFile)
	{
        string strInput;
		getline(myFile, strInput);

        if (strInput.rfind("/score/create/", 0) == 0)
        {
            stringstream stream(strInput);
            string word;
            stream >> word;
            stream >> word;

            meshesNames.push_back(word);
        }
	}
}
