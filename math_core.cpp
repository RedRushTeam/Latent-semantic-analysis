#include "math_core.h"

now_type math_core::divider(int size)
{
	int starter = 1;
	int finisher = size;
	auto sum = (starter + finisher) * (finisher - starter + 1) / 2;
	return sum;
}

void math_core::calculate_max_cont_size()
{
	#pragma omp parallel 
	{
		#pragma omp for schedule(static)
		for (int i = 0; i < this->vec_of_filepaths->size(); ++i) {
			parser _parser((*this->vec_of_filepaths)[i]);	//tut peredaetsa kopiya
			auto result_of_parse = _parser.parse();

			analyzer _analyzer(result_of_parse);
			_analyzer.calculate_counter_of_tokenizer();
		}
	}

	this->max_cont_size = analyzer::counter_of_tokenizer;
}

void math_core::calculate_sample_mean()
{
	this->sample_mean_all = make_shared<piecewise_container_class> (COLLOC_DIST, this->max_cont_size);

	for (int i = 0; this->vec_of_container_classes.size() > i; ++i) {
		*this->sample_mean_all += (vec_of_container_classes[i]);
	}
}

int math_core::get_max_cont_size() const
{
	return this->max_cont_size;
}
