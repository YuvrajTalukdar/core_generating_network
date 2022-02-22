/*
core class handles natural cores
*/

#include"neuron_and_ann_class.h"
#include"core_class.h"

using namespace std;

class segment_class{
    private:
    vector<core_class*> core_vector;
    vector<string> core_save_file_name_vector;

    nn_core_data_package_class* data_pack;
    
    //network information
    float data_division=1.5;
    datapack_structure_defination ds;
    int train_test_predict;
    string network_save_file_name;
    bool id_lock=false;
    int required_no_of_threads;
    int no_of_attributes_per_core_default=10;
    int min_no_of_attributes_per_core=4;
    int extra_attributes_in_last_core=0;
    int no_of_attributes_per_core_balanced;//last core may have few extra attributes.

    //segment identification information
    int segment_no=0,segment_aim=0;//this two must be changed using a function so that proper core is loaded
    string segment_name;
    string segment_save_file_name="NULL";//provided if core is loaded from a core/network savefile. Not set using constructor

    //progress bar data
    int predict_progress_bar_numerator=0;//for the predict progress bar
    int predict_progress_bar_denominator=0;//for the predict progress bar

    void save_segment();

    bool check_if_datapack_has_valid_labels(nn_core_data_package_class* data_pack);//if a label is 0 than it is invalid

    bool load_segment_if_available(int segment_aim,int segment_no,bool file_name_received,string file_name);

    void datapack_analyzer(nn_core_data_package_class* data_pack);//it fills up the datapack_structure_defination ds after analyzing the datapack. 
    struct shuffling_data{
        vector<float> temp_data;
        int temp_label;
    };

    void shuffler(nn_core_filtered_data* f_data);

    void f_data_viewer(string str,vector<nn_core_filtered_data> f_data);

    void filter(nn_core_data_package_class* data_pack,int train_test_predict);

    bool ds_ns_have_same_labels(core_class* core);

    vector<nn_core_filtered_data> f_train_data;//f_test_data; //memory_optimization1
    nn_core_data_package_class test_data;

    vector<vector<nn_core_filtered_data>> f_train_data_split;
    //vector<nn_core_data_package_class> test_data_split;

    void split_attributes_for_each_core();

    void create_cores();

    void train(nn_core_data_package_class* data_pack,bool network_avail_status,int train_test_predict,int no_of_threads);//there cannot be a case of invalid network and data without labels.

    void checker_df(vector<neuron> &output_neurons);

    void checker_nf(vector<neuron> &output_neurons);

    int index_of_neuron_to_be_fired(int label,vector<float> elements);

    vector<neuron> combine_output_neurons(vector<vector<neuron>> output_neuron_matrix);

    int propagate(vector<float> input);

    void test();//parameters not required now
    //memory_optimization1
    void testing_for_each_label(/*,int train_test_predict*/);
    
    void print_prediction(nn_core_data_package_class* data_pack,int train_test_predict);

    void predict_progress_bar();

    void make_prediction_on_user_entered_data();

    string message;
    void print_message();
    void clrscr();

    //test functions
    void display_f_train_data_split()
    {
        for(int a=0;a<f_train_data_split.size();a++)
        {
            cout<<"\n\ncore= "<<a;
            for(int b=0;b<f_train_data_split[a].size();b++)
            {
                cout<<"\nlabel= "<<f_train_data_split[a][b].label;
                for(int c=0;c<f_train_data_split[a][b].data.size()-(f_train_data_split[a][b].data.size()-5);c++)
                {
                    cout<<"\n";
                    for(int d=0;d<f_train_data_split[a][b].data[c].size();d++)
                    {
                        cout<<f_train_data_split[a][b].data[c][d]<<",";
                    }
                }
            }
        }
        
    }

    public:

    void start_segment(int no_of_threads);//train_test_predict=1//train_test_predic is required for extra assurance

    void add_data(nn_core_data_package_class* data_pack,int train_test_predict,float data_division1,string network_save_file_name);
    
    string return_name()
    {   return segment_name;}
    segment_class(int segment_aim1,int segment_no1,string segment_name1);
};