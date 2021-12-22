#include <fstream>

#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

#include <vector>
#include <queue>
#include <set>
#include <string>

using namespace std;

ifstream f_in("input.txt");
ofstream f_out("output.txt");

string protocol = "file://";
string reference_start = "<a href=\"" + protocol;
string reference_end = "\">";

string download = "D:\\test_prog\\";
string upload = "D:\\test_upload\\";

size_t max_threads = thread::hardware_concurrency();

class Crawler
{
	size_t thread_count;
	vector<thread> threads;
	atomic<size_t> workers_number;
	mutex locker;
	queue<string> q;
	set<string> visited;
	long time;

public:

	Crawler(const size_t& threads_number = max_threads) {
		thread_count = threads_number;
		time = 0;
		workers_number = 0;
	}

	void copy_file(const string& file_name) {
		string sas = download + file_name;
		string sos = upload + file_name;
		ifstream t_fin(sas);
		ofstream t_fout(sos);
		t_fout << t_fin.rdbuf();
		t_fin.close();
		t_fout.close();
	}

	string get_file() {
		locker.lock();
		string result = "";
		if (!q.empty())
		{
			result = q.front();
			q.pop();
		}
		locker.unlock();
		return result;
	}

	void find_references(const string& file_name) {
		if (visited.find(file_name) != visited.end())
			return;
		string to_search;
		vector<string> references;
		
		ifstream copying(download + file_name);
		copy_file(file_name);
		getline(copying, to_search);
		copying.close();
		
		for (size_t i = 0; i < to_search.rfind(reference_end); )
		{
			size_t c_ref_start = to_search.find(reference_start, i) + reference_start.length();
			size_t c_ref_end = to_search.find(reference_end, i);
			string c_ref = to_search.substr(c_ref_start, c_ref_end - c_ref_start);
			references.push_back(c_ref);
			i = c_ref_end + 1;
		}
		
		for (const auto& i : references)
		{
			locker.lock();
			if (visited.find(i) == visited.end())
				q.push(i);
			locker.unlock();
		}
		
		locker.lock();
		visited.insert(file_name);
		locker.unlock();
	}

	void work() {
		while (!q.empty() || workers_number)
		{
			string current = get_file();
			if (!current.empty())
			{
				++workers_number;
				find_references(current);
				--workers_number;
			}
		}
	}

	void start(const string& start_url) {
		clock_t start = clock();
		q.push(start_url);
		for (size_t i = 0; i < thread_count; i++)
			threads.push_back(std::thread(&Crawler::work, this));
		for (auto& i : threads)
			i.join();
		clock_t end = clock();
		time = end - start;
	}

	long get_time() {
		return time;
	}

	size_t get_visited_count() {
		return visited.size();
	}
};

int main() {
	string input_filename;
	int thread_count;
	f_in >> input_filename >> thread_count;
	for (size_t i = 0; i < max_threads; i++)
	{
		Crawler virus(i + 1);
		virus.start(input_filename);
		if (i + 1 == thread_count)
			f_out << virus.get_visited_count() << ' ' << virus.get_time();
	}

	f_in.close();
	f_out.close();
	return 0;
}