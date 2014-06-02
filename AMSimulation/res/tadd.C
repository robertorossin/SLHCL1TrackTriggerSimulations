#include "TFileMerger.h"
#include "TString.h"
#include "TSystem.h"
#include "TSystemFile.h"
#include "TSystemDirectory.h"
#include "TList.h"

#include <iostream>
#include <vector>

void tadd(const char *output,
          const char *dirname,
          const char *suffix = ".root",
          const char *prefix = "results")
{
    // from http://root.cern.ch/phpBB3/viewtopic.php?f=3&t=12793
    TSystemDirectory dir(dirname, dirname);
    TList * files = dir.GetListOfFiles();
    std::vector<TString> vec;

    if (files) {
        TSystemFile * file;
        TString fname;
        TString fname_full;
        TIter next(files);
        // Collect the files
        while ( (file = (TSystemFile*)next()) ) {
            fname = file->GetName();
            if (!(file->IsDirectory()) &&
                (!prefix || !(*prefix) || fname.BeginsWith(prefix)) &&
                (!suffix || !(*suffix) || fname.EndsWith(suffix)) ) {
                fname_full = TString(dirname) + "/" + fname;
                vec.push_back(fname_full);
            }
        }

        std::cout << "input directory: " << dirname << std::endl;
        TFileMerger merger(kFALSE);
        merger.OutputFile(output);
        //merger.PrintFiles("true");
        merger.SetFastMethod();
        for (unsigned i=0; i<vec.size(); ++i) {
            bool flag = merger.AddFile(vec.at(i));
            if (flag) {
                std::cout << "  added: " << vec.at(i) << std::endl;
            } else {
                std::cout << "  failed to add: " << vec.at(i) << std::endl;
            }
        }
        std::cout << "output: " << output << std::endl;
        merger.Merge();
        return;
    }
    std::cout << "Failed to get any file from: " << dirname << std::endl;
    return;
}


/*
Example usage
-------------
RES="crab_0_140508_180531/res"
OUT="/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results"
FNAME="SingleMuPlusMinus_OneOverPt_vz0_20140420.root"

mkdir -p "/eos/uscms/$OUT"
root -l -b -q tadd.C+\(\"root://cmseos:1094/$OUT/$FNAME\",\"$RES\",\".root\",\"results\"\)
*/

