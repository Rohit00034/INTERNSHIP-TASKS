#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <chrono>
#include <string>
#include <mutex>
using namespace std;
using namespace chrono;

mutex mtx;

void compressChunk(const string& data,int start,int end,string& result,int index){
string compressed="";
int i=start;
while(i<end){
char current=data[i];
int count=1;
while(i+count<end && data[i+count]==current && count<9){
count++;
}
compressed+=current;
compressed+=to_string(count);
i+=count;
}
lock_guard<mutex> lock(mtx);
result+=compressed;
}

void decompressChunk(const string& data,int start,int end,string& result){
string decompressed="";
for(int i=start;i<end;i+=2){
if(i+1<end){
char ch=data[i];
int count=data[i+1]-'0';
for(int j=0;j<count;j++){
decompressed+=ch;
}
}
}
lock_guard<mutex> lock(mtx);
result+=decompressed;
}

string compressSingleThread(const string& data){
string compressed="";
int i=0;
int size=data.length();
while(i<size){
char current=data[i];
int count=1;
while(i+count<size && data[i+count]==current && count<9){
count++;
}
compressed+=current;
compressed+=to_string(count);
i+=count;
}
return compressed;
}

string decompressSingleThread(const string& data){
string decompressed="";
for(int i=0;i<data.length();i+=2){
if(i+1<data.length()){
char ch=data[i];
int count=data[i+1]-'0';
for(int j=0;j<count;j++){
decompressed+=ch;
}
}
}
return decompressed;
}

string compressMultiThread(const string& data,int numThreads){
string result="";
int chunkSize=data.length()/numThreads;
vector<thread> threads;
vector<string> chunks(numThreads);
for(int i=0;i<numThreads;i++){
int start=i*chunkSize;
int end=(i==numThreads-1)?data.length():(i+1)*chunkSize;
threads.push_back(thread(compressChunk,ref(data),start,end,ref(result),i));
}
for(auto& t:threads){
t.join();
}
return result;
}

string decompressMultiThread(const string& data,int numThreads){
string result="";
int chunkSize=data.length()/numThreads;
if(chunkSize%2!=0) chunkSize++;
vector<thread> threads;
for(int i=0;i<numThreads;i++){
int start=i*chunkSize;
int end=(i==numThreads-1)?data.length():(i+1)*chunkSize;
if(start>=data.length()) break;
threads.push_back(thread(decompressChunk,ref(data),start,end,ref(result)));
}
for(auto& t:threads){
t.join();
}
return result;
}

int main(){
int choice;
string filename,content;
cout<<"File Compression Tool with Multithreading\n";
cout<<"==========================================\n";
while(true){
cout<<"\n1. Compress file (Single Thread)\n";
cout<<"2. Compress file (Multi Thread)\n";
cout<<"3. Decompress file (Single Thread)\n";
cout<<"4. Decompress file (Multi Thread)\n";
cout<<"5. Performance Comparison\n";
cout<<"6. Exit\n";
cout<<"Enter choice: ";
cin>>choice;
if(choice==1){
cout<<"Enter filename to compress: ";
cin>>filename;
ifstream infile(filename);
if(!infile){
cout<<"File not found\n";
continue;
}
content="";
string line;
while(getline(infile,line)){
content+=line;
}
infile.close();
auto start=high_resolution_clock::now();
string compressed=compressSingleThread(content);
auto end=high_resolution_clock::now();
auto duration=duration_cast<milliseconds>(end-start);
ofstream outfile(filename+".compressed");
outfile<<compressed;
outfile.close();
cout<<"Original size: "<<content.length()<<" bytes\n";
cout<<"Compressed size: "<<compressed.length()<<" bytes\n";
cout<<"Time taken: "<<duration.count()<<" ms\n";
cout<<"Compression ratio: "<<(float)compressed.length()/content.length()*100<<"%\n";
}
else if(choice==2){
cout<<"Enter filename to compress: ";
cin>>filename;
cout<<"Enter number of threads (2-8): ";
int threads;
cin>>threads;
if(threads<2) threads=2;
if(threads>8) threads=8;
ifstream infile(filename);
if(!infile){
cout<<"File not found\n";
continue;
}
content="";
string line;
while(getline(infile,line)){
content+=line;
}
infile.close();
auto start=high_resolution_clock::now();
string compressed=compressMultiThread(content,threads);
auto end=high_resolution_clock::now();
auto duration=duration_cast<milliseconds>(end-start);
ofstream outfile(filename+".compressed");
outfile<<compressed;
outfile.close();
cout<<"Original size: "<<content.length()<<" bytes\n";
cout<<"Compressed size: "<<compressed.length()<<" bytes\n";
cout<<"Time taken: "<<duration.count()<<" ms\n";
cout<<"Threads used: "<<threads<<"\n";
}
else if(choice==3){
cout<<"Enter compressed filename: ";
cin>>filename;
ifstream infile(filename);
if(!infile){
cout<<"File not found\n";
continue;
}
content="";
string line;
while(getline(infile,line)){
content+=line;
}
infile.close();
auto start=high_resolution_clock::now();
string decompressed=decompressSingleThread(content);
auto end=high_resolution_clock::now();
auto duration=duration_cast<milliseconds>(end-start);
string outname=filename.substr(0,filename.find(".compressed"))+".decompressed";
ofstream outfile(outname);
outfile<<decompressed;
outfile.close();
cout<<"Decompressed to: "<<outname<<"\n";
cout<<"Time taken: "<<duration.count()<<" ms\n";
}
else if(choice==4){
cout<<"Enter compressed filename: ";
cin>>filename;
cout<<"Enter number of threads (2-8): ";
int threads;
cin>>threads;
if(threads<2) threads=2;
if(threads>8) threads=8;
ifstream infile(filename);
if(!infile){
cout<<"File not found\n";
continue;
}
content="";
string line;
while(getline(infile,line)){
content+=line;
}
infile.close();
auto start=high_resolution_clock::now();
string decompressed=decompressMultiThread(content,threads);
auto end=high_resolution_clock::now();
auto duration=duration_cast<milliseconds>(end-start);
string outname=filename.substr(0,filename.find(".compressed"))+".decompressed";
ofstream outfile(outname);
outfile<<decompressed;
outfile.close();
cout<<"Decompressed to: "<<outname<<"\n";
cout<<"Time taken: "<<duration.count()<<" ms\n";
cout<<"Threads used: "<<threads<<"\n";
}
else if(choice==5){
cout<<"Enter filename for performance test: ";
cin>>filename;
ifstream infile(filename);
if(!infile){
cout<<"File not found\n";
continue;
}
content="";
string line;
while(getline(infile,line)){
content+=line;
}
infile.close();
cout<<"\nPerformance Comparison\n";
cout<<"======================\n";
auto start1=high_resolution_clock::now();
string comp1=compressSingleThread(content);
auto end1=high_resolution_clock::now();
auto time1=duration_cast<milliseconds>(end1-start1);
cout<<"Single Thread: "<<time1.count()<<" ms\n";
auto start2=high_resolution_clock::now();
string comp2=compressMultiThread(content,4);
auto end2=high_resolution_clock::now();
auto time2=duration_cast<milliseconds>(end2-start2);
cout<<"Multi Thread (4): "<<time2.count()<<" ms\n";
if(time2.count()>0){
float speedup=(float)time1.count()/time2.count();
cout<<"Speedup: "<<speedup<<"x faster\n";
cout<<"Performance gain: "<<((speedup-1)*100)<<"%\n";
}
}
else if(choice==6){
cout<<"Exiting program\n";
break;
}
else{
cout<<"Invalid choice\n";
}
}
return 0;
}