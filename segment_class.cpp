#include"segment_class.h"

void segment_class::predict_progress_bar()//need analyzing 
{
    while(shared_block_data_obj.predict_progress_bar_numerator<shared_block_data_obj.predict_progress_bar_denominator)
    {
        float x=shared_block_data_obj.predict_progress_bar_numerator,y=shared_block_data_obj.predict_progress_bar_denominator;
        if(y!=0)
        {
            struct winsize w;
            ioctl(0,TIOCGWINSZ,&w);
            float percentage=(x/y)*100;
            clrscr();
            cout<<"\nprogress: ";
            float hl=w.ws_col/2;
            float ratio=100/hl;
            float pl=percentage*hl/100;
            for(int a=0;a<hl;a++)
            {
                if(a<pl)
                {   cout<<"#";}
                else
                {   cout<<".";}
            }
            cout<<"  "<<percentage<<"%";
            cout<<"  "<<shared_block_data_obj.predict_progress_bar_numerator<<" out of "<<shared_block_data_obj.predict_progress_bar_denominator<<" predictions complete"<<endl<<endl;
            sleep(1);
        }
    }
}

bool segment_class::load_segment_if_available(int segment_aim,int segment_no,bool file_name_received,string file_name)//under construction
{
    return false;
}

void segment_class::checker_df(vector<neuron> &output_neurons)
{
    vector<neuron*> temp_neuron_vect;
    neuron *temp_neuron;
    temp_neuron_vect.clear();
    for(int b=0;b<output_neurons.size();b++)
    {
        //cout<<"\nsummation= "<<output_neurons[b].return_data()<<" label_neuron_to_be_fired_id= "<<label_neuron_to_be_fired_id<<"b= "<<b;
        //cout<<" firing_point= "<<output_neurons[b].return_firing_point();
        if(output_neurons[b].return_fire_status()==true)
        {   temp_neuron_vect.push_back(&output_neurons[b]);}
    }
    //sorting. More optimized sorting algorithm can be used, but if no of output neurons are less than it dose not make a difference.
    for(int b=0;b<temp_neuron_vect.size();b++)
    {
        for(int c=0;c<temp_neuron_vect.size();c++)
        {
            if((temp_neuron_vect[b]->return_data()-temp_neuron_vect[b]->firing_point) > (temp_neuron_vect[c]->return_data()-temp_neuron_vect[c]->firing_point))
            {
                temp_neuron=temp_neuron_vect[b];
                temp_neuron_vect[b]=temp_neuron_vect[c];
                temp_neuron_vect[c]=temp_neuron;
            }
        }
    }
    for(int b=0;b<temp_neuron_vect.size();b++)
    {
        //cout<<"\nsummation= "<<temp_neuron_vect[b]->return_data();
        //cout<<" firing_point= "<<temp_neuron_vect[b]->return_firing_point();
        if(b==0)
        {   temp_neuron_vect[b]->change_fire_status(true);}
        else
        {   temp_neuron_vect[b]->change_fire_status(false);}
    }
}

void segment_class::checker_nf(vector<neuron> &output_neurons)
{
    vector<neuron*> neuron_temp_vect;
    neuron *neuron_temp;
    neuron_temp_vect.clear();
    for(int a=0;a<output_neurons.size();a++)
    {   neuron_temp_vect.push_back(&output_neurons[a]);}
    //sorting
    for(int a=0;a<output_neurons.size();a++)
    {
        for(int b=0;b<output_neurons.size();b++)
        {
            if(neuron_temp_vect[a]->return_data()>neuron_temp_vect[b]->return_data())
            {
                neuron_temp=neuron_temp_vect[a];
                neuron_temp_vect[a]=neuron_temp_vect[b];
                neuron_temp_vect[b]=neuron_temp;
            }
        }
    }
    for(int a=0;a<neuron_temp_vect.size();a++)
    {
        if(neuron_temp_vect[a]->firing_point<100000)
        {   neuron_temp_vect[a]->change_fire_status(true);break;}
    }
}

vector<neuron> segment_class::combine_output_neurons(vector<vector<neuron>> output_neuron_matrix)
{
    vector<neuron> output_neuron;
    output_neuron.resize(output_neuron_matrix[0].size());
    float value_vec[output_neuron.size()]={0};
    //float fp_vec[output_neuron.size()]={0};
    
    for(int a=0;a<output_neuron_matrix.size();a++)
    {
        for(int b=0;b<output_neuron_matrix[a].size();b++)
        {   
            value_vec[b]+=output_neuron_matrix[a][b].return_data();
            output_neuron[b].firing_point+=output_neuron_matrix[a][b].firing_point;
            //fp_vec[b]+=output_neuron_matrix[a][b].firing_point;
        }
    }
    for(int a=0;a<output_neuron.size();a++)
    {   output_neuron[a].set_data(value_vec[a]);}
    return output_neuron;
}

int segment_class::propagate(vector<float> input)//will return the index of 1 fired neuron. This is working for shuttle converted dataset.
{
    int attribute_end_index=0;
    vector<float> trimed_input;
    vector<vector<neuron>> output_neuron_from_each_core;
    for(int a=0;a<core_vector.size();a++)
    {
        trimed_input.clear();
        trimed_input.insert(trimed_input.begin(),input.begin()+attribute_end_index,input.begin()+attribute_end_index+core_vector[a]->return_no_of_input_neuron());
        attribute_end_index=attribute_end_index+core_vector[a]->return_no_of_input_neuron();
        output_neuron_from_each_core.push_back(core_vector[a]->propagate(trimed_input));
    }
    /*cout<<"\n\n";
    for(int a=0;a<output_neuron_from_each_core.size();a++)
    {
        cout<<"\n";
        for(int b=0;b<output_neuron_from_each_core[a].size();b++)
        {
            cout<<"fp="<<output_neuron_from_each_core[a][b].firing_point<<" val="<<output_neuron_from_each_core[a][b].return_data();
        }
    }*/
    vector<neuron> output_neuron=combine_output_neurons(output_neuron_from_each_core);
    output_neuron_from_each_core.clear();
    int no_of_fired_neurons=0;
    for(int a=0;a<output_neuron.size();a++)
    {
        if(output_neuron[a].return_fire_status())
        {   no_of_fired_neurons++;}
    }
    if(no_of_fired_neurons>1)
    {   checker_df(output_neuron);}
    else if(no_of_fired_neurons==0)
    {   checker_nf(output_neuron);}
    int fired_neuron_index,fired_neuron_label;
    int count=0;
    for(int a=0;a<output_neuron.size();a++)
    {
        if(output_neuron[a].return_fire_status())
        {   fired_neuron_index=a;count++;}
    }
    if(count==0)//no fire
    {   fired_neuron_index=-1;}
    else if(count>1)//double fire
    {   fired_neuron_index=-2;}

    return fired_neuron_index;
}

void segment_class::make_prediction_on_user_entered_data()
{
    clrscr();
    char continue1='y';
    vector<float> data_vector;
    float label;
    int fired_neuron_index,correct;
    while(continue1=='y'||continue1=='Y')
    {
        cout<<"\nEnter the "<<ds.no_of_elements_in_each_record<<" digit data: \n";
        for(int a=0;a<ds.no_of_elements_in_each_record;a++)
        {   
            float data;
            cout<<"a"<<a<<"= ";
            cin>>data;
            data_vector.push_back(data);
        }
        fired_neuron_index=propagate(data_vector);
        label=ds.elements[fired_neuron_index];
        cout<<"\nResult = "<<label<<endl;
        data_vector.clear();
        point1:
        cout<<"\nDo you want to make prediction on another data? (y,n)";
        cin>>continue1;
        if(continue1!='N'&&continue1!='n'&&continue1!='y'&&continue1!='Y')
        {
            cout<<"\nWrong option!!!!";
            goto point1;
        }
    }
}

void segment_class::save_segment()//save all the cores and have a segment structure file
{

}

int segment_class::index_of_neuron_to_be_fired(int label,vector<float> elements)
{
    int index;
    for(int a=0;a<elements.size();a++)
    {
        if(elements[a]==label)
        {   index=a;break;}
    }
    return index;
}

void segment_class::test()//finds the overall accuracy of the network
{
    int fired_neuron_index,correct,total=test_data.data.size();
    for(int a=0;a<test_data.data.size();a++)
    {
        fired_neuron_index=propagate(test_data.data[a]);
        if(fired_neuron_index==index_of_neuron_to_be_fired(test_data.labels[a],ds.elements))
        {   correct++;}
    }
    message="\n\nAccuracy= "+to_string((((float)correct)/((float)total))*100)+"%"+" correct= "+to_string(correct)+" total= "+to_string(total);
    print_message();
    int gh;cin>>gh;
}

float segment_class::testing_for_each_label()//finds the accuracy of each label
{
    nn_core_filtered_data f_data;
    vector<nn_core_filtered_data> f_test_data_vector;
    for(int a=0;a<ds.elements.size();a++)
    {
        f_data.data.clear();
        for(int b=0;b<test_data.labels.size();b++)
        {
            if(ds.elements[a]==test_data.labels[b])
            {
                f_data.data.push_back(test_data.data[b]);
                f_data.label=test_data.labels[b];
            }
        }
        f_test_data_vector.push_back(f_data);
    }
    
    int correct_each_label[f_test_data_vector.size()],total_each_label[f_test_data_vector.size()];
    int correct=0,total=0;
    int fired_neuron_index;
    int df[f_test_data_vector.size()]={0},nf[f_test_data_vector.size()]={0};
    for(int a=0;a<f_test_data_vector.size();a++)
    {
        correct_each_label[a]=0;
        total_each_label[a]=0;
        for(int b=0;b<f_test_data_vector[a].data.size();b++)
        {
            fired_neuron_index=propagate(f_test_data_vector[a].data[b]);
            if(fired_neuron_index==index_of_neuron_to_be_fired(f_test_data_vector[a].label,ds.elements))
            {   correct_each_label[a]++;}
            else if(fired_neuron_index==-1)
            {   nf[a]++;}
            else if(fired_neuron_index==-2)
            {   df[a]++;}
            total_each_label[a]++;
        }
        correct+=correct_each_label[a];
        total+=f_test_data_vector[a].data.size();
    }
    float avg_accuracy=0,accuracy;
    for(int a=0;a<f_test_data_vector.size();a++)
    {
        accuracy=(((float)correct_each_label[a])/((float)total_each_label[a]))*100;
        if(critical_variables_set)
        {
            message="\nAccuracy for label "+to_string(f_test_data_vector[a].label)+" = "+to_string(accuracy)+"%"+" correct="+to_string(correct_each_label[a])+" total="+to_string(total_each_label[a])+" df="+to_string(df[a])+" nf="+to_string(nf[a]);
            print_message();
        }
        avg_accuracy+=accuracy;
    }
    avg_accuracy=avg_accuracy/f_test_data_vector.size();
    if(critical_variables_set)
    {
        message="\n\nAvg Accuracy= "+to_string(avg_accuracy)+"%"+" correct= "+to_string(correct)+" total= "+to_string(test_data.data.size());
        print_message();
        message="\n\nTotal Accuracy= "+to_string((((float)correct)/((float)test_data.data.size()))*100)+"%"+" correct= "+to_string(correct)+" total= "+to_string(test_data.data.size());
        print_message();
    }
    return avg_accuracy;
}

void segment_class::print_prediction(nn_core_data_package_class* data_pack,int train_test_predict)//prints the data and predicted label to a file. Also displays the accuracy.
{
    datapack_analyzer(data_pack);
    ofstream file1("prediction_result.csv",ios::out);

    file1<<"data,label,\n";
    float label;
    shared_block_data_obj.predict_progress_bar_denominator=data_pack->data.size();
        
    thread* predict_progress_bar_thread;
    if(pds==true)
    {   predict_progress_bar_thread=new thread(&segment_class::predict_progress_bar,this);}
    
    int fired_neuron_index,correct=0;
    bool has_valid_labels=check_if_datapack_has_valid_labels(data_pack);
    for(int a=0;a<data_pack->data.size();a++)
    {
        for(int b=0;b<data_pack->data[a].size();b++)
        {   file1<<data_pack->data[a][b]<<",";}
        fired_neuron_index=propagate(test_data.data[a]);
        label=ds.elements[fired_neuron_index];
        file1<<":"<<label<<",\n";
        if(has_valid_labels && fired_neuron_index==index_of_neuron_to_be_fired(data_pack->labels[a],ds.elements))
        {   correct++;}
        shared_block_data_obj.predict_progress_bar_numerator++;
    }
    if(pds==true)
    {   predict_progress_bar_thread->join();}
    
    file1.close();
    message="\nPrediction complete, check the file prediction_result.csv\n";
    print_message();
    if(has_valid_labels)
    {
        message="\n\nAccuracy= "+to_string((((float)correct)/((float)data_pack->data.size()))*100)+"%"+" correct= "+to_string(correct)+" total= "+to_string(data_pack->data.size());
        print_message();
    }
}

void segment_class::split_attributes_for_each_core()//ok tested
{
    //split train data
    int start=0,end;
    vector<float> data;
    for(int a=0;a<f_train_data_split.size();a++)//for each core
    {
        for(int b=0;b<f_train_data.size();b++)//for each label
        {
            nn_core_filtered_data f_data;
            f_data.label=f_train_data[b].label;
            int count;
            if(!critical_variables_set)
            {   
                count=f_train_data[b].data.size();
                /*if(f_train_data[b].data.size()<200)
                {   count=f_train_data[b].data.size()/2;}
                else
                {   count=200;}*/
            }
            else
            {   count=f_train_data[b].data.size();}
            for(int c=0;c<count;c++)//for each data
            {
                data.clear();
                for(int d=start;(d<start+no_of_attributes_per_core_balanced && d<f_train_data[b].data[c].size());d++)//for each attribute
                {   data.push_back(f_train_data[b].data[c][d]);}
                if(a==f_train_data_split.size()-1 && extra_attributes_in_last_core>0)
                {
                    for(int d=start+no_of_attributes_per_core_balanced;d<f_train_data[b].data[c].size();d++)
                    {   data.push_back(f_train_data[b].data[c][d]);}
                }
                f_data.data.push_back(data);
            }
            f_train_data_split[a].push_back(f_data);
        }
        start+=no_of_attributes_per_core_balanced;
    }
}

void segment_class::create_cores()
{
    int no_of_cores_required;
    int div=ds.no_of_elements_in_each_record/no_of_attributes_per_core_default;
    int rem=ds.no_of_elements_in_each_record%no_of_attributes_per_core_default;
    if(rem>0 && div>0)
    {   no_of_cores_required=div+1;}
    else if(rem==0 && div>0)
    {   no_of_cores_required=div;}
    else if(rem>0 && div==0)
    {   no_of_cores_required=1;}
    no_of_attributes_per_core_balanced=ds.no_of_elements_in_each_record/no_of_cores_required;
    int remaining_attributes=ds.no_of_elements_in_each_record%no_of_attributes_per_core_balanced;
    if(remaining_attributes>0)
    {   
        div=(ds.no_of_elements_in_each_record-no_of_cores_required*no_of_attributes_per_core_balanced)/no_of_attributes_per_core_balanced;
        rem=(ds.no_of_elements_in_each_record-no_of_cores_required*no_of_attributes_per_core_balanced)%no_of_attributes_per_core_balanced;
        if(div>0)
        {   no_of_cores_required+=(div);}
        if(rem>0)
        {
            if(rem>min_no_of_attributes_per_core)
            {   no_of_cores_required++;}
            else
            {   extra_attributes_in_last_core=rem;}
        }
    }

    for(int a=0;a<no_of_cores_required;a++)
    {
        datapack_structure_defination ds1;
        ds1.elements=ds.elements;
        ds1.no_of_labels=ds.no_of_labels;
        ds1.lower_firing_constrain_rhs=ds.lower_firing_constrain_rhs;
        ds1.upper_not_firing_constrain_rhs=ds.upper_not_firing_constrain_rhs;
        if(a!=no_of_cores_required-1)
        {   ds1.no_of_elements_in_each_record=no_of_attributes_per_core_balanced;}
        else
        {   ds1.no_of_elements_in_each_record=no_of_attributes_per_core_balanced+extra_attributes_in_last_core;}
        core_class* core=new core_class(a,a,segment_aim,segment_no,"core_"+to_string(a),ds1);
        core->set_critical_variable(critical_variable);
        core_vector.push_back(core);
    }
    f_train_data_split.resize(no_of_cores_required);
}

void segment_class::f_data_viewer(string str,vector<nn_core_filtered_data> f_data)
{
    message.clear();
    message="\n"+str+"\n";
    print_message();
    for(int a=0;a<f_data.size();a++)
    {
        message.clear();
        message="\nlabel= "+to_string(f_data[a].label)+" data_size= "+to_string(f_data[a].data.size());
        print_message();
    }
}

bool segment_class::ds_ns_have_same_labels(core_class* core)
{
    if(ds.elements.size()<=core->return_ns().elements.size())
    {   
        int count=0;
        for(int a=0;a<ds.elements.size();a++)
        {
            for(int b=0;b<core->return_ns().elements.size();b++)
            {
                if(ds.elements[a]==core->return_ns().elements[b])
                {   count++;}
            }
        }
        //cout<<"count= "<<count<<" ds= "<<ds.elements.size()<<"ns= "<<ns.elements.size()<<endl;
        if(count==ds.elements.size() && count==core->return_ns().elements.size())
        {   return true;}
        else
        {   return false;}//means current ds is different from previous ds which was used to train the network
    }
    else 
    {   return false;}//means current ds is different from previous ds which was used to train the network
}

void segment_class::shuffler(nn_core_filtered_data* f_data)
{
    vector<shuffling_data> shuffling_data_temp_vector;
    shuffling_data shuffling_data_temp;
    shuffling_data_temp_vector.clear();
    //pushing the data the the shuffling vector
    for(int a=0;a<f_data->data.size();a++)
    {
        shuffling_data_temp.temp_data=f_data->data[a];
        shuffling_data_temp.temp_label=f_data->label;
        shuffling_data_temp_vector.push_back(shuffling_data_temp);
    }
    //shuffeling the data
    random_shuffle(shuffling_data_temp_vector.begin(),shuffling_data_temp_vector.end());
    //pushing the data in the f_data
    f_data->data.clear();
    for(int a=0;a<shuffling_data_temp_vector.size();a++)
    {
        f_data->data.push_back(shuffling_data_temp_vector[a].temp_data);
        f_data->label=shuffling_data_temp_vector[a].temp_label;
    }
}

void segment_class::filter(nn_core_data_package_class* data_pack,int train_test_predict)
{
    nn_core_filtered_data f_data;
    vector<nn_core_filtered_data> f_data_vector;
    for(int a=0;a<ds.elements.size();a++)
    {
        f_data.data.clear();
        for(int b=0;b<data_pack->labels.size();b++)
        {
            if(ds.elements[a]==data_pack->labels[b])
            {
                f_data.data.push_back(data_pack->data[b]);
                f_data.label=data_pack->labels[b];
            }
        }
        f_data_vector.push_back(f_data);
    }
    //calling the shuffler
    for(int a=0;a<f_data_vector.size();a++)
    {
        shuffler(&f_data_vector[a]);//has same size as that is of no of element
    }
    //copying the data to f_train_data and f_test_data
    for(int a=0;a<f_data_vector.size();a++)
    {
        nn_core_filtered_data f_data_temp;
        //copying data to f_train_data
        int limit=0;
        if(train_test_predict==0)//for training
        {   limit=f_data_vector[a].data.size();}
        else if(train_test_predict==1)//for testing
        {   limit=f_data_vector[a].data.size()/data_division;}
        //for(int b=0;b<f_data_vector[a].data.size()/data_division;b++)
        for(int b=0;b<limit;b++)
        {
            f_data_temp.data.push_back(f_data_vector[a].data[b]);
        }
        f_data_temp.label=f_data_vector[a].label;
        f_train_data.push_back(f_data_temp);
        f_data_temp.data.clear();
    }
    //copying the data to the data_pack s
    for(int a=0;a<f_data_vector.size();a++)
    {
        //copying data to testing data
        int begin=0;
        if(train_test_predict==0)//for training 
        {   begin=f_data_vector[a].data.size();}
        else if(train_test_predict==1)//for testing
        {   begin=f_data_vector[a].data.size()/data_division;}
        //for(int b=f_data_vector[a].data.size()/data_division;b<f_data_vector[a].data.size();b++)
        for(int b=begin;b<f_data_vector[a].data.size();b++)
        {
            vector<float> temp;
            for(int c=0;c<f_data_vector[a].data[b].size();c++)
            {
                temp.push_back(f_data_vector[a].data[b][c]);
            }
            test_data.data.push_back(temp);
            temp.clear();
            test_data.labels.push_back(f_data_vector[a].label);
        }
    }
    f_data_vector.clear();//if memory_optimization1 is removed and testing_for_each_label_is_enabled this must be removed
}

void segment_class::datapack_analyzer(nn_core_data_package_class* data_pack)
{
    if(data_pack->analyze_status==false)
    {
        ds.no_of_elements_in_each_record=data_pack->data[0].size();
        vector<int> labels;
        bool found=false;
        for(int a=0;a<data_pack->data.size();a++)
        {
            found=false;
            for(int b=0;b<labels.size();b++)
            {
                if(labels[b]==data_pack->labels[a])
                {
                    found=true;
                    break;
                }
            }
            if(found==false)
            {
                labels.push_back(data_pack->labels[a]);
            }
        }
        ds.no_of_labels=labels.size();
        for(int a=0;a<labels.size();a++)
        {
            ds.elements.push_back(labels[a]);
        }
        data_pack->analyze_status=true;
    }
}

void segment_class::calculate_critical_variables(int no_of_threads)
{
    chromosome critical_variable1=start_genetic_algorithm(10,30,5,no_of_threads);
    critical_variables_set=true;
    set_critical_variable(critical_variable1);
    for(int a=0;a<core_vector.size();a++)
    {   delete core_vector[a];}
    core_vector.clear();
    f_train_data_split.clear();
    cout<<"\nflatening_fx_enabled: "<<critical_variable1.flatening_fx_enabled;
    cout<<"\nzero_weight_remover: "<<critical_variable1.zero_weight_remover;
    cout<<"\nextreame_weight_remover: "<<critical_variable1.extreame_weight_remover;
    cout<<"\nfp_change_value: "<<critical_variable1.fp_change_value;
    cout<<"\nsummation_temp_threshold: "<<critical_variable1.summation_temp_thershold;
    cout<<"\nrhs_upper: "<<critical_variable1.rhs_upper;
    cout<<"\nrhs_lower: "<<critical_variable1.rhs_lower;
    cout<<"\nattributes_per_core: "<<critical_variable1.attributes_per_core;
}

void segment_class::train(int no_of_threads,int train_test_predict,chromosome& current_critical_variable)
{
    if(!critical_variables_set)
    {
        set_critical_variable(current_critical_variable);
        critical_variables_set=false;
        for(int a=0;a<core_vector.size();a++)
        {   delete core_vector[a];}
        core_vector.clear();
        f_train_data_split.clear();
        create_cores();
        split_attributes_for_each_core();
    }
    if(no_of_threads==1)
    {
        for(int a=0;a<core_vector.size();a++)
        {
            if(critical_variables_set)
            {
                message.clear();
                message="\nTraining Core "+to_string(core_vector[a]->return_core_no());
                print_message();
            }
            core_vector[a]->load_training_data_into_core(f_train_data_split[a],no_of_threads);
            core_vector[a]->train_core();
            if(critical_variables_set)
            {   core_vector[a]->save_core();}
            if(critical_variables_set)
            {
                message.clear();
                message="\nCore "+to_string(core_vector[a]->return_core_no())+" training complete.";
                print_message();
            }
        }
    }
    else
    {
        vector<thread*> thread_vec(core_vector.size());
        for(int a=0;a<thread_vec.size();a++)
        {
            if(critical_variables_set)
            {
                message.clear();
                message="\nTraining Core "+to_string(core_vector[a]->return_core_no());
                print_message();
            }
            core_vector[a]->load_training_data_into_core(f_train_data_split[a],no_of_threads);
            thread_vec[a]=new thread(&core_class::train_core,core_vector[a]);
        }
        for(int a=0;a<thread_vec.size();a++)
        {   
            thread_vec[a]->join();
            if(critical_variables_set)
            {
                message.clear();
                message="\nCore "+to_string(core_vector[a]->return_core_no())+" training complete.";
                print_message();
            }
        }
        if(critical_variables_set)
        {
            for(int a=0;a<core_vector.size();a++)
            {   core_vector[a]->save_core();}
        }
    }
    float avg_accuracy=0;
    if(train_test_predict==1 || !critical_variables_set)
    {   
        //test();
        avg_accuracy=testing_for_each_label();
        current_critical_variable.fitness=avg_accuracy;
    }
}

void segment_class::start_trainer(nn_core_data_package_class* data_pack,bool network_avail_status,int train_test_predict,int no_of_threads)
{
    datapack_analyzer(data_pack);//function checked!
    //filtering data according to labels
    filter(data_pack,train_test_predict);//f_train_data pack gets created here.
    data_pack->data.clear();
    data_pack->labels.clear();
    bool network_loaded_correctly=true;
    if(network_avail_status)
    {
        for(int a=0;a<core_vector.size();a++)
        {
            if(!ds_ns_have_same_labels(core_vector[a]))
            {
                message.clear();
                message="\n\nERROR!! core number "+to_string(core_vector[a]->return_core_no())+" failed to load correctly.";
                print_message();
                network_loaded_correctly=false;
                break;
            }
        }
        message.clear();
        message="\nNetwork loading successful.Training process starting...";
        print_message();
    }
    if(network_loaded_correctly)
    {
        if(!critical_variables_set)
        {
            calculate_critical_variables(no_of_threads);
        }
        if(!network_avail_status)
        {
            message.clear();
            message="\nNetwork not loaded, so training freshly created network....";
            print_message();
        }
        save_segment();
        message.clear();
        message="\nnetwork saved";
        print_message();
        auto start = high_resolution_clock::now();
        //training by threading can be done here in future
        if(core_vector.size()==0)//this means core save file not found and fresh cores need to be created
        {   
            f_train_data_split.clear();
            create_cores();
        }
        split_attributes_for_each_core();
        f_train_data.clear();
        train(no_of_threads,train_test_predict,critical_variable);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        message.clear();
        message="\n\nTime Taken= "+to_string(duration.count()/pow(10,6))+"\n\n";
        print_message();
    }
}

bool segment_class::check_if_datapack_has_valid_labels(nn_core_data_package_class* data_pack)
{
    if(data_pack->data.size()>data_pack->labels.size())
    {   return false;}
    else
    {   return true;}
}

void segment_class::print_message()
{
    pthread_mutex_lock(&lock_1);
    cout<<message;
    //cout<<"\ncore_no= "<<core_no<<"core_name="<<core_name<<" started...";
    pthread_mutex_unlock(&lock_1);
}

void segment_class::start_segment(int no_of_threads)//train_test_predict=1//train_test_predic is required for extra assurance
{
    message.clear();
    message="\nsegment_no= "+to_string(segment_no)+" segment_name="+segment_name+" datapack_size= "+to_string((int)data_pack->data.size())+" started...";
    print_message();
    if(check_if_datapack_has_valid_labels(data_pack)==true && train_test_predict<2)//true
    {
        bool network_avail_status=load_segment_if_available(0,0,false,"");
        start_trainer(data_pack,network_avail_status,train_test_predict,no_of_threads);
    }
    else if(train_test_predict==3)//making prediction on used entered individual data.
    {
        bool network_load_status=load_segment_if_available(0,0,true,network_save_file_name);
        if(network_load_status==true)
        {   
            message.clear();
            message="Network successfully loaded";
            print_message();
            make_prediction_on_user_entered_data();
        }
        else
        {   
            message.clear();
            message="ERROR!!! failed to load network from the network file.";
            print_message();
        }
    }
    else if(train_test_predict==4)//auto training mode
    {
        start_trainer(data_pack,false,train_test_predict,no_of_threads);
    }
    else
    {
        if(load_segment_if_available(0,0,true,network_save_file_name)==true && train_test_predict==2)//network avail,vaild label not avail,predic mode
        {   
            print_prediction(data_pack,train_test_predict);
        }
        else
        {   
            message.clear();
            message="Invalid option";
            print_message();
        }//ERROR!!
    }
}

void segment_class::add_data(nn_core_data_package_class* data_pack1,int train_test_predict1,float data_division2,string network_save_file_name1)
{
    data_pack=data_pack1;
    train_test_predict=train_test_predict1;
    data_division=data_division2;
    network_save_file_name=network_save_file_name1;
    cout<<"\ndata_pack= "<<data_pack->data.size()<<" train_test_predict="<<train_test_predict<<" data_division="<<data_division<<" save_file_name="<<network_save_file_name;
}

void segment_class::set_critical_variable(chromosome critical_variable1)
{
   critical_variables_set=true;
   no_of_attributes_per_core_default=critical_variable1.attributes_per_core;
   ds.lower_firing_constrain_rhs=critical_variable1.rhs_lower;
   ds.upper_not_firing_constrain_rhs=critical_variable1.rhs_upper;
   critical_variable=critical_variable1;
   //for(int a=0;a<core_vector.size();a++)
   //{    core_vector[a]->set_critical_variable(critical_variable1);}
}

segment_class::segment_class(int segment_aim1,int segment_no1,string segment_name1)
{
    if(id_lock==false)
    {
        segment_aim=segment_aim1;
        segment_name=segment_name1;
        id_lock=true;
    }
    else
    {  
        message.clear();
        message="Failed to set core number and core aim as id_lock=true";
        print_message();
    }
}

void segment_class::clrscr()
{
    cout << "\033[2J\033[1;1H";
    //system("clear");
}