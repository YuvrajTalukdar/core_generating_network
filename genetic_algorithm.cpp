#include"segment_class.h"

int segment_class::get_random_number(int min,int max)
{
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> dist6(min,max); // distribution in range [1, 6]
    return dist6(rng);
}

bool segment_class::get_random_bool()
{
    int num=get_random_number(0,1);
    if(num==0)
    {   return false;}
    else
    {   return true;}
}

bool segment_class::comparator(chromosome c1,chromosome c2)
{
    if(c1.fitness>c2.fitness)
    {   return true;}
    else
    {   return false;}
}

void segment_class::mutation(vector<chromosome>& population)
{
    vector<int> index_temp;
    bool found;
    for(int a=0;a<population.size();a++)
    {
        index_temp.clear();
        for(int b=0;b<no_of_genes_to_mutate;b++)
        {
            point1:
            int index=get_random_number(0,7);
            found=false;
            for(int c=0;c<index_temp.size();c++)
            {
                if(index_temp[c]==index)
                {   found=true;break;}
            }
            if(found)
            {   goto point1;}
            switch (index)
            {
                case 0:
                population[a].flatening_fx_enabled=!population[a].flatening_fx_enabled;
                break;
                case 1:
                population[a].extreame_weight_remover=!population[a].extreame_weight_remover;
                break;
                case 2:
                population[a].zero_weight_remover=!population[a].zero_weight_remover;
                break;
                case 3:
                population[a].fp_change_value=get_random_number(fp_change_value_min,fp_change_value_max);
                break;
                case 4:
                population[a].summation_temp_thershold=get_random_number(summation_temp_thershold_min,summation_temp_thershold_max);
                break;
                case 5:
                population[a].rhs_upper=get_random_number(rhs_upper_min,rhs_upper_max);
                break;
                case 6:
                population[a].rhs_lower=get_random_number(rhs_lower_min,rhs_lower_max);
                break;
                case 7:
                population[a].attributes_per_core=get_random_number(attributes_per_core_min,attributes_per_core_max);
                break;
            }
            index_temp.push_back(index);
        }
    }
}

void segment_class::chromosome_data_transfer(int crossover_index,bool before,chromosome& source,chromosome& destination)
{
    if(before)
    {
        switch (crossover_index)
        {
            case 0:
            destination.flatening_fx_enabled=source.flatening_fx_enabled;
            break;
            case 1:
            destination.flatening_fx_enabled=source.flatening_fx_enabled;
            destination.extreame_weight_remover=source.extreame_weight_remover;
            break;
            case 2:
            destination.flatening_fx_enabled=source.flatening_fx_enabled;
            destination.extreame_weight_remover=source.extreame_weight_remover;
            destination.zero_weight_remover=source.zero_weight_remover;
            break;
            case 3:
            destination.flatening_fx_enabled=source.flatening_fx_enabled;
            destination.extreame_weight_remover=source.extreame_weight_remover;
            destination.zero_weight_remover=source.zero_weight_remover;
            destination.fp_change_value=source.fp_change_value;
            break;
            case 4:
            destination.flatening_fx_enabled=source.flatening_fx_enabled;
            destination.extreame_weight_remover=source.extreame_weight_remover;
            destination.zero_weight_remover=source.zero_weight_remover;
            destination.fp_change_value=source.fp_change_value;
            destination.summation_temp_thershold=source.summation_temp_thershold;
            break;
            case 5:
            destination.flatening_fx_enabled=source.flatening_fx_enabled;
            destination.extreame_weight_remover=source.extreame_weight_remover;
            destination.zero_weight_remover=source.zero_weight_remover;
            destination.fp_change_value=source.fp_change_value;
            destination.summation_temp_thershold=source.fp_change_value;
            destination.rhs_upper=source.rhs_upper;
            break;
            case 6:
            destination.flatening_fx_enabled=source.flatening_fx_enabled;
            destination.extreame_weight_remover=source.extreame_weight_remover;
            destination.zero_weight_remover=source.zero_weight_remover;
            destination.fp_change_value=source.fp_change_value;
            destination.summation_temp_thershold=source.fp_change_value;
            destination.rhs_upper=source.rhs_upper;
            destination.rhs_lower=source.rhs_lower;
            break;
        }
    }
    else
    {
        switch (crossover_index)
        {
            case 0:
            destination.extreame_weight_remover=source.extreame_weight_remover;
            destination.zero_weight_remover=source.zero_weight_remover;
            destination.fp_change_value=source.fp_change_value;
            destination.summation_temp_thershold=source.fp_change_value;
            destination.rhs_upper=source.rhs_upper;
            destination.rhs_lower=source.rhs_lower;
            destination.attributes_per_core=source.attributes_per_core;
            break;
            case 1:
            destination.zero_weight_remover=source.zero_weight_remover;
            destination.fp_change_value=source.fp_change_value;
            destination.summation_temp_thershold=source.fp_change_value;
            destination.rhs_upper=source.rhs_upper;
            destination.rhs_lower=source.rhs_lower;
            destination.attributes_per_core=source.attributes_per_core;
            break;
            case 2:
            destination.fp_change_value=source.fp_change_value;
            destination.summation_temp_thershold=source.fp_change_value;
            destination.rhs_upper=source.rhs_upper;
            destination.rhs_lower=source.rhs_lower;
            destination.attributes_per_core=source.attributes_per_core;
            break;
            case 3:
            destination.summation_temp_thershold=source.fp_change_value;
            destination.rhs_upper=source.rhs_upper;
            destination.rhs_lower=source.rhs_lower;
            destination.attributes_per_core=source.attributes_per_core;
            break;
            case 4:
            destination.rhs_upper=source.rhs_upper;
            destination.rhs_lower=source.rhs_lower;
            destination.attributes_per_core=source.attributes_per_core;
            break;
            case 5:
            destination.rhs_lower=source.rhs_lower;
            destination.attributes_per_core=source.attributes_per_core;
            break;
            case 6:
            destination.attributes_per_core=source.attributes_per_core;
            break;
        }
    }
}

vector<chromosome> segment_class::crossover(vector<chromosome>& population)
{
    vector<chromosome> new_gen;
    for(int a=0;a<population.size();a+=2)
    {
        if(a!=population.size()-1)
        {
            int crossover_index=get_random_number(0,6);//0 to crossover_index. If crossover_index = 6 than [0,1,2,3,4,5,6] && [7]
            chromosome new_chromosome1,new_chromosome2;
            //tranfer chromosome a data
            new_chromosome1.id=current_chromosome_id;
            current_chromosome_id++;
            chromosome_data_transfer(crossover_index,true,population[a],new_chromosome1);
            chromosome_data_transfer(crossover_index,false,population[a+1],new_chromosome1);
            new_gen.push_back(new_chromosome1);
            
            new_chromosome2.id=current_chromosome_id;
            current_chromosome_id++;
            chromosome_data_transfer(crossover_index,false,population[a],new_chromosome2);
            chromosome_data_transfer(crossover_index,true,population[a+1],new_chromosome2);
            new_gen.push_back(new_chromosome2);
        }
        else
        {   new_gen.push_back(population[a]);}
    }
    return new_gen;
}

vector<chromosome> segment_class::tournament_selection(vector<chromosome> population)
{
    vector<chromosome> selected_population;
    while(population.size()!=0)
    {
        int index1=get_random_number(0,population.size()-1);
        point1:
        int index2=get_random_number(0,population.size()-1);
        if(index1==index2)
        {   goto point1;}
        if(population[index1].fitness>population[index2].fitness)
        {
            selected_population.push_back(population[index1]);
            if(index1>index2)
            {
                population.erase(population.begin()+index1);
                population.erase(population.begin()+index2);
            }
            else
            {
                population.erase(population.begin()+index2);
                population.erase(population.begin()+index1);
            }
        }
        else
        {
            selected_population.push_back(population[index2]);
            if(index1>index2)
            {
                population.erase(population.begin()+index1);
                population.erase(population.begin()+index2);
            }
            else
            {
                population.erase(population.begin()+index2);
                population.erase(population.begin()+index1);
            }
        }
        if(population.size()==1)
        {   
            selected_population.push_back(population[0]);
            population.pop_back();
        }
    }
    return selected_population;
}

void segment_class::generate_initial_population()
{
    for(int a=0;a<population_size;a++)
    {
        chromosome c1;
        c1.id=a;
        c1.flatening_fx_enabled=get_random_bool();
        c1.extreame_weight_remover=get_random_bool();
        c1.zero_weight_remover=get_random_bool();
        c1.fp_change_value=get_random_number(fp_change_value_min,fp_change_value_max);
        c1.summation_temp_thershold=get_random_number(summation_temp_thershold_min,summation_temp_thershold_max);
        c1.rhs_upper=get_random_number(rhs_upper_min,rhs_upper_max);
        c1.rhs_lower=get_random_number(rhs_lower_min,rhs_lower_max);
        c1.attributes_per_core=get_random_number(attributes_per_core_min,attributes_per_core_max);
        population.push_back(c1);
    }
    current_chromosome_id=population_size;
}

void segment_class::calc_fitness_threaded(int no_of_threads,vector<chromosome>& population)
{
    for(int a=0;a<population.size();a++)
    {
        save_chromosome(population[a]);
        try{
            train(no_of_threads,1,population[a]);
        }
        catch(exception &e)
        {
            cout<<"\nCrashed!!";
            save_chromosome(population[a]);
            int gh;cin>>gh;
        }
        //cout<<"\na= "<<a<<" f= "<<population[a].fitness;
    }
    /*if(no_of_threads==1)
    {
        
    }
    else
    {
        
    }*/
}

chromosome segment_class::start_genetic_algorithm(int pop_size,int iterations,int mutation_percentage,int no_of_threads)
{
    population_size=pop_size;
    no_of_genes_to_mutate=8*mutation_percentage/100;
    if(no_of_genes_to_mutate==0)
    {   no_of_genes_to_mutate=3;}
    else if(no_of_genes_to_mutate>8)//to handle more than 100 percent condition, which is illegal.
    {   no_of_genes_to_mutate=3;}
    cout<<"\nthreads= "<<no_of_threads;
    generate_initial_population();
    //print_population(population);
    //fitness calculation
    cout<<"\n\nCalculation initial population fitness...";
    calc_fitness_threaded(no_of_threads,population);
    int total_value=0;
    for(int a=0;a<iterations;a++)
    {
        //selection
        vector<chromosome> selected_parents=tournament_selection(population);
        //crossover
        vector<chromosome> new_gen=crossover(selected_parents);
        selected_parents.clear();
        //mutation
        mutation(new_gen);
        //fitness calculation
        calc_fitness_threaded(no_of_threads,new_gen);
        //adding new gen to population pool
        population.insert(population.end(),new_gen.begin(),new_gen.end());
        //sorting population pool according to fitness
        sort(population.begin(),population.end(),comparator);
        //removing the last 50 chromosome with lowest fitness
        population.erase(population.end()-(population.size()-population_size),population.end());
        total_value=0;
        for(int b=0;b<population.size();b++)
        {   
            //cout<<"\nfitness= "<<population[b].fitness;
            total_value+=population[b].fitness;
        }
        cout<<"\nIteration: "<<a<<", total_population_fitness: "<<total_value<<", max_fitness: "<<population[0].fitness;
    }
    return population[0];
}

void segment_class::print_population(vector<chromosome>& population)
{
    for(int a=0;a<population.size();a++)
    {
        cout<<"\n"<<population[a].flatening_fx_enabled<<","<<population[a].extreame_weight_remover<<","<<population[a].zero_weight_remover<<","<<population[a].fp_change_value<<","<<population[a].summation_temp_thershold<<","<<population[a].rhs_upper<<","<<population[a].rhs_lower<<","<<population[a].attributes_per_core;
    }
}

void segment_class::save_chromosome(chromosome& chromosome)
{
    ofstream file1("critical_var.txt",ios::out);
    file1<<"\nflatening_fx_enabled: "<<chromosome.flatening_fx_enabled;
    file1<<"\nzero_weight_remover: "<<chromosome.zero_weight_remover;
    file1<<"\nextreame_weight_remover: "<<chromosome.extreame_weight_remover;
    file1<<"\nfp_change_value: "<<chromosome.fp_change_value;
    file1<<"\nsummation_temp_threshold: "<<chromosome.summation_temp_thershold;
    file1<<"\nrhs_upper: "<<chromosome.rhs_upper;
    file1<<"\nrhs_lower: "<<chromosome.rhs_lower;
    file1<<"\nattributes_per_core: "<<chromosome.attributes_per_core;
    file1.close();
}