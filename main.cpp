#include <iostream>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <fstream>
#include "csvstream.hpp"

using namespace std;

class Classifier{
   private: 
      map<pair<string, string>, int> my_map; //<<label, word>, count>
      set<string> setof_unique_tags;
      set<string> setof_all_unique_words;
      int total_posts;
      int vocabulary_count;
      map<string, int> tag_count;


   public:

    //REQUIRE: str to not be empty
    //MODIFY: N/A
    //EFFECT: Returns a set of strings of unique (non-duplicate) words.
    set<string> unique_words(const string &str) {
        istringstream source(str);
        set<string> words;
        string word;
        while (source >> word) {
            words.insert(word);
        }
        return words;
    }

    //REQUIRE: label and content to not be NULL
    //MODIFY: my_map
    //EFFECT: Initializes my_map with the data from the training sets; Increase the number
    //        of posts; Increase the count of a tag.
    void init_mymap (const string &label, const string &content){
        setof_unique_tags.insert(label);

        set<string> setof_unique_words;
        setof_unique_words = unique_words(content);

        for (auto x: setof_unique_words) {
            my_map[{label, x}]++;
            setof_all_unique_words.insert(x);
        }

        total_posts++;
        tag_count[label]++;
        
    }

    //REQUIRE: word to not be NULL
    //MODIFY: N/A
    //EFFECT: Returns the number of posts containing word w. 
    int total_posts_containing_word (const string &word){
        int count = 0;
        for (auto x : setof_unique_tags){
            if (my_map.find(make_pair(x, word)) != my_map.end()) {
                count += my_map.find(make_pair(x, word))->second;
            }
        }
        return count;
    }

    //REQUIRE: label to not be NULL
    //MODIFY: N/A
    //EFFECT: returns ln(P(C))
    double lnPC (string label){

        double total_posts_d = (double)total_posts;
        double total_post_labelC = (double)tag_count[label];

        return log(total_post_labelC/total_posts_d);
    }

    //REQUIRE: label and string to not be NULL
    //MODIFY: N/A
    //EFFECT: returns ln(P(W|C))
    double lnPWC (const string &label, const string &word){
        int pair_count;
        if (my_map.find(make_pair(label, word)) != my_map.end()){
            pair_count = my_map.find(make_pair(label, word))->second;
        } else {
            pair_count = -1;
        }

        double pair_count_d = (double)pair_count;
        double total_posts_d = (double)total_posts;
        double number_posts_w = total_posts_containing_word (word);   //probably need to double it?
        double total_post_labelC = tag_count[label];

        if (number_posts_w == 0){
            return log (1.0 / total_posts_d);
        } else if (pair_count == -1){
            return log (number_posts_w / total_posts_d);
        } else {
            return log (pair_count_d / total_post_labelC);
        }
    }
    
    //REQUIRE: content to not be NULL
    //MODIFY: N/A
    //EFFECT: returns a dataset pair of all tags and its respective logscore
    vector <pair<string, double>> tags_and_logscore (const string &content){
        vector<pair<string, double>> tandl;
        
        set<string> setof_words = unique_words (content);

        for (auto x : setof_unique_tags) {
            double tag_score = 0.0;
            tag_score += lnPC(x);
            for (auto y : setof_words) {
                tag_score += lnPWC(x, y); 
            }
            tandl.push_back(make_pair(x,tag_score));
        }

        return tandl;
    }

    //REQUIRE: content to not be NULL
    //MODIFY: N/A
    //EFFECT: returns the highest scoring tag as the predicted tag and its log probability 
    pair<string, double> predicted_tag_and_logprob (vector <pair<string, double>> &tandl){  

        string highestscoringtag;
        double highestscore = -99999999.0;

        for (auto pr : tandl) {
            if (pr.second > highestscore) {
                highestscoringtag = pr.first;
                highestscore = pr.second;
            } else if (pr.second == highestscore) {
                if (highestscoringtag.compare(pr.first) > 0) {
                    highestscoringtag = pr.first;
                    highestscore = pr.second;
                }
            }
        }
        return make_pair(highestscoringtag, highestscore);
    }
    
    //REQUIRE: N/A
    //MODIFY: N/A
    //EFFECT: prints the additional details in debug mode. 
    void debugmode_print (){
        
        vocabulary_count = setof_all_unique_words.size();

        cout << "vocabulary size = " << vocabulary_count << "\n\nclasses:\n";

        for (auto x : setof_unique_tags) {
            cout << "  " << x << ", " << tag_count[x] << " examples, log-prior = " 
                                                               << lnPC(x) << "\n";
        }
        
        cout << "classifier parameters:" << '\n';

        for (auto y : my_map) {
            cout << "  " << y.first.first << ":" << y.first.second 
                         << ", count = " << y.second << ", log-likelihood = " 
                         << lnPWC(y.first.first, y.first.second) << "\n";
        }
        cout << "\n";
    }

    //REQUIRE: N/A
    //MODIFY: N/A
    //EFFECT: returns the total_posts.
    int get_total_posts (){
        return total_posts;
    }
};

int main (int argc, char *argv[]){
    
    cout.precision(3);
    Classifier classi = Classifier ();

    string train_file = string(argv[1]);
    csvstream csvtrain(train_file);
    string test_file = string(argv[2]);
    csvstream csvtest(test_file);
    bool debugmode = false;

    if (argc != 3 && argc != 4) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
    } else if ((argc == 4)&&(string(argv[3]) == "--debug")){
        debugmode = true; 
        cout << "training data:\n";
    } else if ((argc == 4)&&(string(argv[3]) != "--debug")){
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
    }
    
    map<string, string> row;
    while (csvtrain >> row) {
       string tag; 
       string content;
       for (auto &col : row) {
            if (col.first == "tag") {     
                tag = col.second;
            } else if (col.first == "content") {
                content = col.second;
            }
        }
        classi.init_mymap(tag, content);
        if (debugmode){
            cout << "  label = " << tag << ", content = " << content << '\n';
        }
    }
    
    cout << "trained on " << classi.get_total_posts() << " examples" << '\n';
    if (debugmode) { 
        classi.debugmode_print();
    } else {
        cout << "\n";
    }
    cout << "test data:\n";

    int total_posts_tested = 0;
    int total_posts_accurate = 0;
    while(csvtest >> row){
        string tag; 
        string content;
        for (auto &col : row) {
            if(col.first == "tag"){
                tag = col.second;
            } else if(col.first == "content") {
                content = col.second;
            }
        }
        
        vector <pair<string, double>> dataset = classi.tags_and_logscore(content);
        pair <string, double> prediction = classi.predicted_tag_and_logprob(dataset);
        cout << "  correct = " << tag << ", predicted = " << prediction.first   
                           << ", log-probability score = " << prediction.second
                           << "\n  content = " << content << "\n\n";
        
        total_posts_tested++;
        if (tag == prediction.first){
            total_posts_accurate++;
        }
    }

    cout << "performance: " << total_posts_accurate << " / " << total_posts_tested <<
                                                        " posts predicted correctly\n";  

}