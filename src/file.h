class file{
    double TIMESTEP;
    string filePath;
    string fileName;
    string fileType;
    string folderName;
    string outFolderDir;
    string outFileDir;
    string SuccFolderDir;
    string unSuccFolderDir;
    int n_events;
    int n_dPoints;
    bool read_check = false;
    string plot_opt;

    int n_succesfulFits = 0;
    string scintillator;
    string source;
    string date;
    string detector;
    int set_no;
    double ampRatio;
    double threshold;
    double samplingRate = 400e6;
    int n_segment;

    
    vector<double> time_code = {};
    vector<vector<double>> matrix; 
    vector<vector<double>> fit_output = {{}, {}, {}, {}, {}, {}};
    vector<int> SuccFitsInd;
    vector<TGraph> SuccFitsGraphs;
    vector<int> unSuccFitsInd;
    vector<TGraph> unSuccFitsGraphs;
    vector<TGraph> fitGraphs;
    vector<TFitResultPtr> fitPResVec = {};

    TTree* fTree = new TTree("fTree", "Metadata Tree");
    TGraph *fgraph = new TGraph();
    TF1 *land_fit = new TF1 ("land_fit","([0]*TMath::Landau(x,[1],[2]) + [3])");
    TCanvas *c = new TCanvas("c");
    
    public:
        void errReadFirst(){
            cout << YELLOW << "ERROR: Read and Fit the data first." << endl;
        }

        void errFillHistFirst(){

            cout << YELLOW << "ERROR: Fill the histograms first to write its output." << endl;           
        }

        file(string filePath_){
            filePath = filePath_;

            c->SetCanvasSize(640, 512);

            // Marker settings for global graph pointers
            fgraph->SetMarkerStyle(20); 
            fgraph->SetMarkerSize(0.4);
            fgraph->SetMarkerColorAlpha(4, 0.5);

            land_fit->SetLineWidth(1);
            land_fit->SetLineColorAlpha(kRed, 0.7);

            // Obtains the file name and its extension, without its directory.
            size_t slashPos_1 = filePath.find_last_of("/\\");
            fileName = filePath.substr(slashPos_1 + 1);
            
            // Obtains the file extension. (txt), (pdf) etc.
            size_t lastDotPos = filePath.find_last_of('.');
            fileType = filePath.substr(lastDotPos + 1);

            size_t slashPos_2 = filePath.substr(0, slashPos_1).find_last_of("/\\");
            folderName = filePath.substr(slashPos_2 + 1, slashPos_1 - slashPos_2 - 1);
            
            outFolderDir = filePath.substr(0, filePath.size() - fileName.size() - folderName.size() - 6) + "out/" + folderName;
            if ((fileType == "txt")||(fileType == "h5")){
                istringstream iss(fileName);
                string temp;

                while (getline(iss, temp, '_')) {
                    if((temp.find("Cs") != -1) || (temp.find("Ba") != -1) || (temp.find("Co") != -1 || (temp.find("NoSource") != -1))){
                        source = temp;
                    }
                    else if ((temp.find("EJ") != -1) || (temp.find("CR") != -1)){
                        scintillator = temp;
                    }
                    else if(temp.find("Seg") != -1){
                        string temp_str = temp.substr(0, temp.size() - 3);
                        n_segment = stoi(temp_str);
                    }
                    else if(temp.find("Amp") != -1){
                        string temp_str = temp.substr(3, temp.size() - 3);
                        ampRatio = stod(temp_str);
                    }
                    else if(temp.find("Th") != -1){
                        string temp_str = temp.substr(2, temp.size() - 4);
                        threshold = stod(temp_str);
                    }
                    else if((temp.find("SiPM") != -1) || (temp.find("PMT") != -1)){
                        detector = temp;
                    }
                    else if(temp.find("MSps") != -1){
                        string temp_str = temp.substr(0, temp.size() - 4);
                        samplingRate = stod(temp_str)*1e6;
                    }
                    else if(temp.find("Sample") != -1){
                        string temp_str = temp.substr(0, temp.size() - 6);
                        n_dPoints = stoi(temp_str);
                    }
                    else if(temp.find("XXXX") != -1){
                        scintillator = temp;
                    }
                    else {
                        set_no = stoi(temp);
                    }
                    
                }
                read_check = true;

                size_t underPos = folderName.find_last_of("_");
                date = folderName.substr(0, underPos) + "/" + folderName.substr(underPos + 1);

                outFileDir = outFolderDir + "/" + fileName.substr(0, fileName.size() - fileType.size() - 1);

                if (!fs::exists(outFileDir)) {
                    fs::create_directories(outFileDir);
                }

            }
        }       
        
        string GetFileName(){
            if (read_check){
                return fileName;
            }

            else{
                errReadFirst();
            }
        }
        
        string GetoutFolderDir(){
            if (read_check){
                return outFolderDir;
            }

            else{
                errReadFirst();
            }            
        }

        string GetFileType(){
            if (read_check){
                return fileType;
            }

            else{
                errReadFirst();
            }
        }

        vector<vector<double>> GetMatrix(){
            if (read_check){
                return matrix;
            }

            else{
                errReadFirst();
            }
        }
        
        /// @brief  Returns analyzed data of all events in the file.
        vector<vector<double>> GetFitResults(){
            if (read_check){
                return fit_output;
            }

            else{
                errReadFirst();
            }
        }
        
        string GetScintillator(){
            if (read_check){
                return scintillator;
            }
            else{
                errReadFirst();
            }
        }

        string GetSource(){
            if (read_check){
                return source;
            }
            else{
                errReadFirst();
            }
       }

        string GetDate(){
            if (read_check){
                return date;
            }
            else{
                errReadFirst();
            }
        }

        int GetSetNo(){
            if (read_check){
                return set_no;
            }
            else{
                errReadFirst();
            }
        }

        double GetThreshold(){
            if (read_check){
                return threshold;
            }
            else{
                errReadFirst();
            }
        }
        
        vector<double> GetTimeCode(){
            return time_code;
        }

        int GetsuccessfulFits(){
            if (read_check){
                return n_succesfulFits;
            }
            else{
                errReadFirst();
            }
        }

        int GetNDataPoints(){
            return n_dPoints;
        }

        string GetPlotOpt(){
            return plot_opt;
            }

        /// @brief Changes the time-step value. Default value is 2.5e-9.
        void ChangeTimeStep(double new_step){
            TIMESTEP = new_step;
        }
        
        void SetPlotOpt(string new_plot_opt){
            if (GetPlotOpt() != new_plot_opt){
                plot_opt = new_plot_opt;
            }
        }
        
        bool IsValidType(){
            if((fileType == "txt")||(fileType == "h5")){
                return true;
            }
            else{
                cout << RED << "File type is not valid: ." << YELLOW  << fileType << RESET << endl;
                return false;
            }
        }
        
        /// @brief Reads the file located in the given path. Transposes and stores the data in it as file.matrix variable.
        void ReadFile(){
            cout << GREEN << "Reading: " << RESET << folderName << "/" << fileName; 
            TIMESTEP = 1.0/samplingRate;
            if (fileType == "txt"){ // .txt file reader.
                ifstream inpFile;
                inpFile.open(filePath);

                string line;
                double value;

                while (getline(inpFile, line)){
                    istringstream iss(line);
                    int i_col = 0;
                    if (matrix.empty()){
                        while (iss >> value) {
                            matrix.push_back({-1*value});
                        }
                    }
                    else {
                        while (iss >> value) {
                            matrix[i_col].push_back(-1*value);
                            i_col += 1;
                        }
                    }
                }
                n_events = matrix.size();
                n_dPoints = matrix[0].size();
            }  
            else if (fileType == "h5"){ // .h5 file reader.

                H5File hFile(filePath, H5F_ACC_RDONLY);
                string dsetHead = "/Waveforms/Channel 1/Channel 1 Seg";
                string dsetTail = "Data";

                Group count_group = hFile.openGroup("/Waveforms/Channel 1");

                hsize_t size = count_group.getNumObjs();
                int n_events = int(size);

                for (int i = 0; i < n_events; i++){
                    string datasetPath = dsetHead + to_string(i + 1) + dsetTail;

                    DataSet dset = hFile.openDataSet(datasetPath.c_str());

                    DataSpace dspace = dset.getSpace();

                    hsize_t dims[2];
                    dspace.getSimpleExtentDims(dims, NULL);
                    hsize_t dimsm[1];
                    dimsm[0] = dims[0];
                    DataSpace memspace(1, dimsm);

                    vector<double> temp;
                    temp.resize(dims[0]);
                    dset.read(temp.data(), PredType::NATIVE_DOUBLE, memspace, dspace);

                    for (int j = 0; j < dims[0]; j++) {
                        temp[j] *= -1;
                    }

                    matrix.push_back(temp);

                    if (n_dPoints != dims[0]){
                        n_dPoints = dims[0];
                    }
                }
            }
            for (int i = 0; i < n_dPoints; i++){
                time_code.push_back(TIMESTEP*i);
            }
        }
        
        /// @brief Takes the matrix obtained by ReadFile() argument and applies Landau fit to each event in the matrix. Stores important variables like rise time, fall time, peak voltage etc. Get those results by using GetFitResults() method.
        void LandFit(){
            int ind = 0;
            double fit_range_time = 160e-9;
            int fit_range = fit_range_time/TIMESTEP;
            double sTime;
            double fTime;

            for (vector<double> event: matrix){

                auto maxVoltIt = max_element(event.begin(), event.end());
                double maxVolt = *maxVoltIt;
                int maxVoltInd = distance(event.begin(), maxVoltIt);
                double maxVoltTime = TIMESTEP*maxVoltInd;


                if(maxVoltTime - fit_range_time*0.4 >= 0){
                    sTime = maxVoltTime - fit_range_time*0.4;
                }
                else {
                    sTime = 0;
                }
                if(maxVoltTime + fit_range_time*0.6 <= n_dPoints*TIMESTEP){
                    fTime = maxVoltTime + fit_range_time*0.6;
                }
                else{
                    fTime = n_dPoints*TIMESTEP;
                }

                fgraph->Set(event.size());
                for(int i = 0; i < event.size(); i++){
                    fgraph->SetPoint(i, time_code.data()[i], event.data()[i]);
                    }
                fgraph->SetTitle("");

                land_fit->SetRange(sTime, fTime); 
                land_fit->SetParameters(maxVolt*5.0, maxVoltInd*TIMESTEP, TIMESTEP*(maxVolt+1.0), 0);

                land_fit->SetParLimits(0, maxVolt*3, maxVolt*7.5);
                land_fit->SetParLimits(1, maxVoltInd*TIMESTEP*0.90, maxVoltInd*TIMESTEP*1.10);
                land_fit->SetParLimits(2, TIMESTEP*(maxVolt+1.0)*0.3, TIMESTEP*maxVolt*100.0);

                TFitResultPtr fit_result = fgraph->Fit(land_fit, "QR");
                fitPResVec.push_back(fit_result);

                vector<double> peak = {land_fit->GetMaximumX(), land_fit->GetMaximum()};
                
                double v_high = peak[1]*0.8;
                double v_low = peak[1]*0.2;

                double rise_l = land_fit->GetX(v_low, sTime, peak[0]);
                double rise_h = land_fit->GetX(v_high, sTime, peak[0]);
                double fall_l = land_fit->GetX(v_low, peak[0], fTime);
                double fall_h = land_fit->GetX(v_high, peak[0], fTime);

                double rise_t = rise_h - rise_l;
                double fall_t = fall_l - fall_h;

                if (fit_result == 4 || rise_t <= 0 || fall_t <= 0 ) {
                    unSuccFitsInd.push_back(ind);
                    TGraph gr_temp = *fgraph;
                    fitGraphs.push_back(gr_temp);
                    unSuccFitsGraphs.push_back(gr_temp);
                    /*
                    double er = 0.0005;
                    cerr << RED << ind << RESET << " --- " << YELLOW << chi2/ndf << RESET;

                    if(land_fit->GetParameter(0)/maxVolt < 3 + er|| land_fit->GetParameter(0)/maxVolt > 7.5 - er || land_fit->GetParameter(1)/(maxVoltInd*TIMESTEP) < 0.90 + er|| land_fit->GetParameter(1)/(maxVoltInd*TIMESTEP) > 1.10 - er|| land_fit->GetParameter(2)/(TIMESTEP*maxVolt) < 0.3 + er || land_fit->GetParameter(2)/(TIMESTEP*maxVolt) > 100.0 - er){
                        cerr << " --- " << YELLOW << land_fit->GetParameter(0)/maxVolt << RESET << " --- " << YELLOW << land_fit->GetParameter(1)/(maxVoltInd*TIMESTEP) << RESET << " --- " << YELLOW << land_fit->GetParameter(2)/(TIMESTEP*maxVolt);
                    }
                    cerr << RESET << endl;
                    */
                }
                else{
                    n_succesfulFits += 1;
                    double chi2 = land_fit->GetChisquare();
                    double ndf = land_fit->GetNDF();
                    double auc = land_fit->Integral(sTime, fTime);

                    fit_output[0].push_back(rise_t);
                    fit_output[1].push_back(fall_t);
                    fit_output[2].push_back(auc);
                    fit_output[3].push_back(peak[0]);
                    fit_output[4].push_back(peak[1]);
                    fit_output[5].push_back(chi2/ndf);

                    SuccFitsInd.push_back(ind);
                    TGraph gr_temp = *fgraph;
                    fitGraphs.push_back(gr_temp);
                    SuccFitsGraphs.push_back(gr_temp);

                    /*
                    double er = 0.0005;
                    cerr << GREEN << ind << RESET << " --- " << land_fit->GetParameter(0)/maxVolt << RESET << " --- " << YELLOW << land_fit->GetParameter(1)/(maxVoltInd*TIMESTEP) << RESET << " --- " << YELLOW << land_fit->GetParameter(2)/(TIMESTEP*maxVolt) << RESET << endl;

                    

                    if(land_fit->GetParameter(0)/maxVolt < 3 + er|| land_fit->GetParameter(0)/maxVolt > 7.5 - er || land_fit->GetParameter(1)/(maxVoltInd*TIMESTEP) < 0.90 + er|| land_fit->GetParameter(1)/(maxVoltInd*TIMESTEP) > 1.10 - er|| land_fit->GetParameter(2)/(TIMESTEP*maxVolt) < 0.3 + er || land_fit->GetParameter(2)/(TIMESTEP*maxVolt) > 100.0 - er){
                        cerr << " --- " << YELLOW << land_fit->GetParameter(0)/maxVolt << RESET << " --- " << YELLOW << land_fit->GetParameter(1)/(maxVoltInd*TIMESTEP) << RESET << " --- " << YELLOW << land_fit->GetParameter(2)/(TIMESTEP*maxVolt) << RESET << " --- ";
                    }
                    cerr << RESET << " --- " << BLUE << fit_result << RESET << endl;
                    */
                }
                ind += 1;;
            }
            fgraph->Set(0);
            land_fit->SetParameters(0,0,0,0);
        }
        
        /// @brief Returns successfull fits to all fits ratio in the string form.  
        string successfulFitRate(){
            if (n_events != 0){
                return to_string(n_succesfulFits) + "/" + to_string(n_events);
            }
            else{
                cout << YELLOW << "ERROR: Read and Fit the data first." << endl;
            }

        }
        /*
        void FillHistos(){
            if (fit_output.empty()){
                errFillHistFirst();
            }
            
            else {
                vector<double> bounds;
                for (int i = 0; i < fit_output.size(); i++){

                    double d_min = *min_element(fit_output[i].begin(), fit_output[i].end());
                    double d_max = *max_element(fit_output[i].begin(), fit_output[i].end());
                    bounds.push_back(d_min);
                    bounds.push_back(d_max);
                }

                TH1F *hist_rise_t = new TH1F("hist_rise_t", "Rise Time", 40, bounds[0], bounds[1]);
                TH1F *hist_fall_t = new TH1F("hist_fall_t", "Fall Time", 40, bounds[2], bounds[3]);
                TH1F *hist_auc = new TH1F("hist_auc", "Area Under the Curve", 40, bounds[4], bounds[5]);
                TH1F *hist_peak_x = new TH1F("hist_peak_x", "Peak Time", 40, bounds[6], bounds[7]);
                TH1F *hist_peak_y = new TH1F("hist_peak_y", "Peak Voltage", 40, bounds[8], bounds[9]);

                hist_rise_t->FillN(n_succesfulFits, &fit_output[0][0], NULL);
                hist_fall_t->FillN(n_succesfulFits, &fit_output[1][0], NULL);
                hist_auc->FillN(n_succesfulFits, &fit_output[2][0], NULL);
                hist_peak_x->FillN(n_succesfulFits, &fit_output[3][0], NULL);
                hist_peak_y->FillN(n_succesfulFits, &fit_output[4][0], NULL);

                histograms.push_back(hist_rise_t);
                histograms.push_back(hist_fall_t);
                histograms.push_back(hist_auc);
                histograms.push_back(hist_peak_x);
                histograms.push_back(hist_peak_y);
            }
        }
        */

        void WriteOutFit(int i){
            fgraph = &fitGraphs[i];

            string title = "Event No: " + to_string(i);
            
            string savePath = to_string(i) + ".pdf";
            
            c->SetTitle(title.c_str());
            fgraph->SetTitle(title.c_str());
            fgraph->GetXaxis()->SetTitle("Time (s)");
            fgraph->GetYaxis()->SetTitle("Voltage (V)");

            fgraph->Draw("AP");

            TFitResultPtr temp_fitRes = fitPResVec[i];
            c->SaveAs(savePath.c_str());

            c->Clear();
            fgraph->Set(0);
        }

        void WriteOutGraphs(){
            SuccFolderDir = outFileDir + "/SuccFits";
            unSuccFolderDir = outFileDir + "/unSuccFits";
            if (GetPlotOpt() == "a"){
                for (int i = 0; i < unSuccFitsGraphs.size(); i++){
                    if (!fs::exists(unSuccFolderDir)) {
                        fs::create_directories(unSuccFolderDir);
                    }
                    fgraph = &(unSuccFitsGraphs[i]);

                    string title = "Event No: " + to_string(unSuccFitsInd[i]);
                    string savePath = unSuccFolderDir + "/" + to_string(unSuccFitsInd[i]) + ".pdf";
                    
                    c->SetTitle(title.c_str());
                    fgraph->SetTitle(title.c_str());
                    fgraph->GetXaxis()->SetTitle("Time (s)");
                    fgraph->GetYaxis()->SetTitle("Voltage (V)");
                    fgraph->Draw("AP");

                    c->SaveAs(savePath.c_str());

                    c->Clear();
                    fgraph->Set(0);
                }               
                for (int i = 0; i < SuccFitsGraphs.size(); i++){
                    if (!fs::exists(SuccFolderDir)) {
                        fs::create_directories(SuccFolderDir);
                    }

                    fgraph = &(SuccFitsGraphs[i]);

                    string title = "Event No: " + to_string(SuccFitsInd[i]);
                    string savePath = SuccFolderDir + "/" + to_string(SuccFitsInd[i]) + ".pdf";
                    
                    c->SetTitle(title.c_str());
                    fgraph->SetTitle(title.c_str());
                    fgraph->GetXaxis()->SetTitle("Time (s)");
                    fgraph->GetYaxis()->SetTitle("Voltage (V)");
                    fgraph->Draw("AP");

                    c->SaveAs(savePath.c_str());

                    fgraph->Set(0);
                    c->Clear();
                }
            }
            else if (GetPlotOpt() == "s"){
                for (int i = 0; i < SuccFitsGraphs.size(); i++){
                    if (!fs::exists(SuccFolderDir)) {
                        fs::create_directories(SuccFolderDir);
                    }
                    
                    fgraph = &(SuccFitsGraphs[i]);

                    string title = "Event No: " + to_string(SuccFitsInd[i]);
                    string savePath = SuccFolderDir + "/" + to_string(SuccFitsInd[i]) + ".pdf";
                    
                    c->SetTitle(title.c_str());
                    fgraph->SetTitle(title.c_str());
                    fgraph->GetXaxis()->SetTitle("Time (s)");
                    fgraph->GetYaxis()->SetTitle("Voltage (V)");
                    fgraph->Draw("AP");

                    c->SaveAs(savePath.c_str());

                    fgraph->Set(0);
                    c->Clear();
                }
            }
            else if (GetPlotOpt() == "u"){
                for (int i = 0; i < unSuccFitsGraphs.size(); i++){
                    if (!fs::exists(unSuccFolderDir)) {
                        fs::create_directories(unSuccFolderDir);
                    }

                    fgraph = &(unSuccFitsGraphs[i]);

                    string title = "Event No: " + to_string(unSuccFitsInd[i]);
                    string savePath = unSuccFolderDir + "/" + to_string(unSuccFitsInd[i]) + ".pdf";
                    
                    c->SetTitle(title.c_str());
                    fgraph ->SetTitle(title.c_str());
                    fgraph ->GetXaxis()->SetTitle("Time (s)");
                    fgraph ->GetYaxis()->SetTitle("Voltage (V)");
                    fgraph ->SetMarkerStyle(20); 
                    fgraph ->SetMarkerSize(1);
                    fgraph ->SetMarkerColor(9); 
                    fgraph ->Draw("AP");

                    c->SaveAs(savePath.c_str());

                    c->Clear();
                    fgraph->Set(0);
                }                   
            }
        }

        void WriteOut(){
            TFile *outFile = new TFile((outFileDir + "/output.root").c_str(), "RECREATE");

            int temp_n_succesfulFits;
            string temp_scintillator;
            string temp_source;
            string temp_date;
            string temp_detector;
            int temp_set_no;
            double temp_ampRatio;
            double temp_threshold;
            double temp_samplingRate;
            int temp_n_segment;
            int temp_n_dPoints;

            vector<double> temp_rise_t;
            vector<double> temp_fall_t;
            vector<double> temp_auc;
            vector<double> temp_peak_x;
            vector<double> temp_peak_y;
            vector<double> temp_x2_ndf;

            fTree->Branch("n_succesfulFits", &n_succesfulFits);
            fTree->Branch("scintillator", &temp_scintillator);
            fTree->Branch("source", &temp_source);
            fTree->Branch("date", &temp_date);
            fTree->Branch("detector", &temp_detector);
            fTree->Branch("set_no", &temp_set_no);
            fTree->Branch("ampRatio", &temp_ampRatio);
            fTree->Branch("threshold", &temp_threshold);
            fTree->Branch("samplingRate", &temp_samplingRate);
            fTree->Branch("n_segment", &temp_n_segment);
            fTree->Branch("n_dPoints", &temp_n_dPoints);

            fTree->Branch("rise_t", &temp_rise_t);
            fTree->Branch("fall_t", &temp_fall_t);
            fTree->Branch("auc", &temp_auc);
            fTree->Branch("peak_x", &temp_peak_x);
            fTree->Branch("peak_y", &temp_peak_y);
            fTree->Branch("chi2/ndf", &temp_x2_ndf);


            temp_n_succesfulFits = n_succesfulFits;
            temp_scintillator = scintillator;
            temp_source = source;
            temp_date = date;
            temp_detector = detector;
            temp_set_no = set_no;
            temp_ampRatio = ampRatio;
            temp_threshold = threshold;
            temp_samplingRate = samplingRate;
            temp_n_segment = n_segment;
            temp_n_dPoints = n_dPoints;

            temp_rise_t = fit_output[0];
            temp_fall_t = fit_output[1];
            temp_auc = fit_output[2];
            temp_peak_x = fit_output[3];
            temp_peak_y = fit_output[4];
            temp_x2_ndf = fit_output[5];
            
            fTree->Fill();
            fTree->Write();
            outFile->Close();

            WriteOutGraphs();
            
            outFile->Close();
            delete fTree;
            delete land_fit;
        }
};