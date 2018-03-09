#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

template <class Type>
class MCST; // minimum cost spanning tree

template <class Type>
class group{
	friend class MCST<Type>; // minimum cost spanning tree can get private member in group
	template <class U>
	friend void quick_sort_group(group<U> *arr,int start,int end); // Quick Sort function to sort array of groups
	template <class U>
	friend group<U> median(group<U> x,group<U> y,group<U> z); // Median function to return the median group with median cost in three groups 
	private:
		Type num1,num2; // number1 and number2
		Type cost; // the cost between number1 and number2
		int field; // the link which these two number belongs to
	public:
		group(Type a,Type b,Type c)
		{
			num1=a;
			num2=b;
			cost=c;
			field=0; // initial value of field set to zero
		}
		group()
		{
		}
		void set(Type a,Type b,Type c)
		{
			num1=a;
			num2=b;
			cost=c;
			field=0; // initial value of field set to zero
		}
		void print(void)
		{
			cout << num1 << " " << num2 << " " << cost << " \n" << endl;
		}
};

template <class Type>
void quick_sort_group(group<Type> *arr,int start,int end) // Quick Sort function to sort array of groups
{
	if(start<end){
		group<Type> pivot_value=arr[start];
		int left=start+1,right=end;
		while(1){
			while(left<=end&&arr[left].cost<=pivot_value.cost)
				left++;
			while(start<right&&pivot_value.cost<=arr[right].cost)
				right--;
			if(left>right)
				break;
			swap(arr[left],arr[right]);	
		}
		swap(arr[start],arr[right]);
		quick_sort_group(arr,start,right-1);
		quick_sort_group(arr,right+1,end);
	}
}

template <class Type>
class MCST{
	public:
		MCST(int m=2) // initial maximum numbers in Minimum Cost Spanning Tree set to 2
		{
			arr=new group<Type> [m];
			max=m;
			num=0; // total node numbers in Minimum Cost Spanning Tree, initial value equals to zero
			field_num=1; // initial : The Minimum Cost Spanning Tree has one link , so field number = 1
		}
		
		void build(group<Type> *input_arr,int how_much)
		{
			quick_sort_group(input_arr,0,how_much-1); // First, sort the input : group array 
			for(int i=0;i<how_much;i++){ 
				// For all groups in sorted array, take out groups inorder and determine if it will build a circle with the groups in MCST ( Minimum Cost Spanning Tree )
				if(!is_circle(input_arr[i]))
					insert(input_arr[i]); // if the group won't cause a circle, insert it
			}
		}
		
		void insert(group<Type> in) // insert a group
		{
			if(num+1==max){ // if Minimum Cost Spanning Tree is full, double the size of it
				max*=2;
				group<Type> *temp_arr=new group<Type> [max];
				
				for(int i=0;i<num;i++)
					temp_arr[i]=arr[i];
				delete arr;
				arr=temp_arr;
			}
			arr[num]=in; // add group in Minimum Cost Spanning Tree
			for(int i=0;i<num;i++){
				// Find which link (field) does the new group belongs to
				// If the numbers of the new group is same as the number of a old group, they have the same link (field) 
				if(arr[i].num1==arr[num].num1||arr[i].num1==arr[num].num2||arr[i].num2==arr[num].num1||arr[i].num2==arr[num].num2){
					if(arr[num].field==0) // the new group doesn't belongs to any link (field) before, so it belongs to this link (field)
						arr[num].field=arr[i].field;
					else{ // the new group belongs to a link (field) before, so we union two links together
						for(int j=0;j<num;j++){
							if(arr[j].field==arr[i].field)
								arr[j].field=arr[num].field;
						}
					}
				}
			}
			if(arr[num].field==0) // the new group doesn't belongs to any link (field) before, and it doesn't has any semi-number in MCST, so it belongs to a new link
				arr[num++].field=field_num++;
			else // the new group belongs to a link (field) before
				num++;
			return;
		}
		
		bool is_circle(group<Type> in) // to consider if adding this new group will cause a circle
		{
			int same[2][field_num]; // array same consider field x has how many same numbers
			for(int i=0;i<field_num;i++){
				same[0][i]=0; // initial set to zero ( each field has zero same numbers )
				same[1][i]=0;
			}
			for(int i=0;i<num;i++){
				if(arr[i].num1==in.num1||arr[i].num2==in.num1)
					same[0][arr[i].field]++; // if has same number then add same[ field number ] for once
				else if(arr[i].num1==in.num2||arr[i].num2==in.num2)
					same[1][arr[i].field]++;
			}
			for(int i=0;i<field_num;i++){
				if(same[0][i]>=1&&same[1][i]>=1) // has over two same numbers in the link ( will cause a circle )
					return true; // has a circle
			}
			return false; // no circle
		}
		
		void print(void)
		{
			int length=0;
			for(int i=0;i<num;i++){
				arr[i].print();
				length+=arr[i].cost;
			}
			cout << length << endl;	
		}
		
	private:
		group<Type> *arr;
		int max;
		int num;
		int field_num;
};

int main(void)
{
	int size=2; // initial input array size
	MCST<int> tree;
	group<int> *arr=new group<int>[size];
	int total=0;
	
	string read_file_name; // input file name
	cin >> read_file_name; 
	
	ifstream ifile(read_file_name.c_str(),ios::in);
	if(ifile.is_open()){ // determine if the input file has been opened successfully
		while(!ifile.eof()){ // if not reach the end of file , keep on getting datas
			if(total+1==size){ // if input array full , double its size
				size*=2;
				group<int> *temp_arr=new group<int>[size];
				for(int i=0;i<=total;i++)
					temp_arr[i]=arr[i];
				delete arr;
				arr=temp_arr;
			}
			int num1,num2,cost;
			ifile >> num1 >> num2 >> cost;
			arr[total++].set(num1,num2,cost); // add data to input array members 
		}
		tree.build(arr,total); // build a Minimum Cost Spanning Tree of input array ( graph )
		tree.print(); // print the Minimum Cost Spanning Tree
	}
	else
		cout << "File not found...\n";
	
	return 0;
} 
