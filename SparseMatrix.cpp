#include <iostream>
#include <string>
using namespace std;


class NodeType {
public:
	int info;
	NodeType * link;
	int column_index;
};


class SM{

public:		
	int n; int m;							// # rows, # columns	
	NodeType * *RowsPtr;					// array of pointers
	int i_row; int i_column; int i_info;	//user input: row , column , info
	
	SM(int rows, int columns){					// constructor
		if ((rows <= 0) || (columns <= 0)){
			cout << "unable to construct matrix!" << endl;
			abort();
		}

		this->n = rows;
		this->m = columns;
		RowsPtr = new NodeType*[rows]{};
		for (int x = 0; x < rows; x++){
			RowsPtr[x] = new NodeType;
			RowsPtr[x] = NULL;
		}
	}
	
	~SM(){								//destructor
		for (int i = 0; i < n; i++){
			if (RowsPtr[i] == NULL){
				delete RowsPtr[i];
			}
			else{
				NodeType * current = RowsPtr[i];
				while (current->link != NULL){
					current = current->link;
					delete RowsPtr[i];
					RowsPtr[i] = current;
				}
			}
		}
		delete[] RowsPtr;
	}

	SM(SM* other){							//copy constructor , modified with SM(*) to suit my code
		n = other->n;
		m = other->m;
		RowsPtr = new NodeType*[n];			// create a copy of an array of pointers

		for (int i = 0; i < n; i++){				// copy all the elements
			RowsPtr[i] = new NodeType();
			
			if (other->RowsPtr[i] == NULL){			//copy empty rows
				RowsPtr[i] = NULL;
			}
			else{
				NodeType *current = other->RowsPtr[i];			// current tranverse the original "other" matrix
				NodeType *tail = RowsPtr[i];					// tail added copied element into the back of new matrix row
				
				while (current != NULL){
					if (current->link != NULL){
						NodeType *newNode = new NodeType;
						newNode->column_index = current->column_index;
						newNode->info = current->info;
						newNode->link = NULL;
						
						tail = newNode;
						tail = tail->link;
					}
					current = current->link;
				}
			}
		}
	}

	void setInsertElem(int row, int column, int info){
		if ((row <= 0 || column <= 0) || (row > n || column> m)){		//validate inserted value row and column number
			cout << "Out of bound !" << endl;
			abort();
		}
	
		this->i_row = row;					// save input element
		this->i_column = column;
		this->i_info = info;
	}

	void readElements(){	
		int x = i_row - 1;							// decrease input row index value by 1 (for array)

		if (RowsPtr[x] == NULL){					// when row[x] is empty 
			NodeType *newNode = new NodeType;
			newNode->column_index = i_column;
			newNode->link = NULL;
			newNode->info = i_info;
			RowsPtr[x] = newNode;
		}	
		else {										// when row[x] is not empty
			NodeType * current = this->RowsPtr[x];											

			while (current != NULL){				//transverse the row

				//insert in the back of current node, when current-> last node
				if ((i_column > current->column_index) && (current->link == NULL)){	
					NodeType *newNode = new NodeType;
					newNode->column_index = i_column;
					newNode->info = i_info;
					newNode->link = NULL;
					current->link = newNode;
					break;
				}
				// replace current->node's info
				else if (i_column == (current->column_index)){				
					current->info = i_info;
				}
				// insert in the begining of list
				else if (i_column < current->column_index){					
					NodeType *newNode = new NodeType;
					newNode->column_index = i_column;
					newNode->info = i_info;
					newNode->link = current;
					current = newNode;
					RowsPtr[x] = current;
					break;
				}
				//  insert between current and current next
				else if ((i_column > current->column_index) && (i_column < current->link->column_index)){	
					NodeType *newNode = new NodeType;
					newNode->column_index = i_column;
					newNode->info = i_info;
					newNode->link = current->link;
					current->link = newNode;
					RowsPtr[x] = current;
					break;
				}
				current = current->link;
			}
		}
	}

	void printMatrix(){					// show the matrix

		if (RowsPtr == NULL){
			cout << "Matrix not found!" << endl;
		}

		for (int i = 0; i < n; i++){
			NodeType * current = RowsPtr[i];

			for (int k = 1; k <= m; k++){
				if (current == NULL) {
					cout << "  0";
				}
				else if (current->column_index == k){
					cout << "  " << current->info;

					current = current->link;
				}
				else if (current->column_index != k){
					cout << "  0";
				}
			}
			cout << endl;
		}
	}
	
	void addSM(SM * other){														// add two matrix together
		if ((this->n != other->n) || (this->m != other->m)){
			cout << "Unable to add two matrix with different size !" << endl;
		}
		else{
					
			for (int i = 0; i < n; i++){										// looping row index			
				NodeType *current_1 = this->RowsPtr[i];							// for first SM transverse
				NodeType *current_2 = other->RowsPtr[i];						// for second SM tranverse
			
				if ((current_1 == NULL) && (current_2 != NULL)){				//  if row_1st is empty but row_2nd is not
				
					while (current_2 != NULL){									//populating whole row_2nd into row_1st
						NodeType *newNode = new NodeType;
						newNode->column_index = current_2->column_index;
						newNode->info = current_2->info;
						newNode->link = NULL;

						if (current_1 == NULL){
							current_1 = newNode;
							current_2 = current_2->link;
							RowsPtr[i] = current_1;
						}
						else{
							current_1->link = newNode;

							current_2 = current_2->link;
						}
					}
				}			
				else if ((current_1 == NULL) && (current_2 == NULL)){		// do nothing when both row in empty

				}		
				else if ((current_1 != NULL) && (current_2 != NULL)){		// both row is not empty
					
					// looping column index
					for (int k = 1; k <= m; k++){							
						
						//when both current_1 and current_2 has the same number as columns index
						if ((current_1->column_index == k) && (current_2->column_index == k)){		

							// addition of info between current_1 and current_2
							current_1->info = (current_1->info + current_2->info);

							//when current_1 is the last node of row_1 , and current_2 in row_2 is not the last node
							if ((current_1->link == NULL) && (current_2->link != NULL)){

								// populating the nodes left in row_2nd in to row 1st
								while (current_2 != NULL){
									NodeType *newNode = new NodeType;
									newNode->column_index = current_2->column_index;
									newNode->info = current_2->info;
									newNode->link = NULL;
									current_1->link = newNode;

									current_1 = current_1->link;
									current_2 = current_2->link;
								}
								break;
							}
							// when current_2 in row_2 is the last node , the column index loops break
							else if (current_2->link == NULL){
								break;
							}
							// when both current_1 and current_2 is not last node
							else{
								current_1 = current_1->link;
								current_2 = current_2->link;
							}
						}

						// when current_1 is EQUAL to k but current_2 is LARGER than k
						else if ((current_1->column_index == k) && (current_2->column_index > k)){

							// when current_1 and current_2 are the last node
							if ((current_1->link == NULL) && (current_2->link == NULL)){
								NodeType *newNode = new NodeType;
								newNode->column_index = current_2->column_index;
								newNode->info = current_2->info;
								newNode->link = NULL;
								current_1->link = newNode;

								break;
							}

							//when current_1 is the last node of row_1st , and current_2 in row_2nd is not the last node
							else if ((current_1->link == NULL) && (current_2->link != NULL)){

								// populating the nodes left in row_2nd in to row_1st
								while (current_2 != NULL){
									NodeType *newNode = new NodeType;
									newNode->column_index = current_2->column_index;
									newNode->info = current_2->info;
									newNode->link = NULL;
									current_1->link = newNode;

									current_1 = current_1->link;
									current_2 = current_2->link;
								}
								break;
							}

							// tranverse to next node on row_1st
							else {
								current_1 = current_1->link;
							}
						}

						// current_1 column index is LARGER than k, while current_2 is EQUAL to k
						else if ((current_1->column_index > k) && (current_2->column_index == k)){

							// general instantiation
							NodeType *newNode = new NodeType;
							newNode->column_index = k;
							newNode->info = current_2->info;

							// if both current_1 and current_2 is last node
							if ((current_1->link == NULL) && (current_2->link == NULL)){
								
								// inserting first element into row_1
								if (current_1->column_index == RowsPtr[i]->column_index){
									newNode->link = current_1;
									current_1 = newNode;
									RowsPtr[i] = current_1;

									break;
								}

								// inserting other than first element into the row_1
								else{
									newNode->link = current_1;
									current_1 = newNode;

									break;
								}
							}

							// when current_1 is the last node of row_1 , and current_2 in row_2 is NOT the last node
							else if ((current_1->link == NULL) && (current_2->link != NULL)){

								// inserting current_2 as first element into row_1st
								if (current_1->column_index == RowsPtr[i]->column_index){
									newNode->link = current_1;
									current_1 = newNode;
									RowsPtr[i] = current_1;

									current_1 = current_1->link;
									current_2 = current_2->link;
								}

								// inserting current_2 as other element into the row_1st
								else{
									newNode->link = current_1;
									current_1 = newNode;

									current_1 = current_1->link;
									current_2 = current_2->link;
								}
							}

							// when current_1 is the NOT last node of row_1st , and current_2 in row_2nd is the LAST node
							else if ((current_1->link != NULL) && (current_2->link == NULL)){
								
								// inserting current_2 as first element into row_1st
								if (current_1->column_index == RowsPtr[i]->column_index){
									newNode->link = current_1;
									current_1 = newNode;
									RowsPtr[i] = current_1;

									break;
								}
								// inserting current_2 as other element into the row_1st
								else{
									newNode->link = current_1;
									current_1 = newNode;

									break;
								}
							}
						}
					}
				}
			}
			cout << "Final matrix :" << endl;
			this->printMatrix();
		}
	}
};


void main(){
	int rows, columns, info;
	char y;

	cout << "Please insert the number of rows and columns for Matrix 1 : (eg: 3 5)" << endl;
	cin >> rows >> columns;
	cout << endl;

	SM *matrix1 = new SM(rows, columns);

	while (true){
		cout << "Please insert the number of row ,column and value accordingly to insert new element into Matrix 1 : (eg: 3 4 5)" << endl;
		cin >> rows >> columns >> info;
		matrix1->setInsertElem(rows, columns, info);
		matrix1->readElements();

		cout << endl;
		cout << "Value inserted." << endl;
		cout << endl;
		cout << "Are you finish entering the elements? Y/N" << endl;
		cin >> y;
		if (y == 'Y' || y == 'y'){
			break;
		}
		cout << endl;
	}
	cout << endl;
	cout << endl;
	cout << "Final Matrix 1" << endl;
	matrix1->printMatrix();
	cout << endl;

	cout << "Please insert the number of rows and columns for Matrix 2 : (eg: 3 5)" << endl;
	cin >> rows >> columns;
	cout << endl;

	SM *matrix2 = new SM(rows, columns);

	while (true){
		cout << "Please insert the number of row ,column and value accordingly to insert new elelment into Matrix 2 :" << endl;
		cin >> rows >> columns >> info;
		matrix2->setInsertElem(rows, columns, info);
		matrix2->readElements();

		cout << endl;
		cout << "Value inserted." << endl;
		cout << endl;
		cout << "Are you finish entering the elements? Y/N" << endl;
		cin >> y;
		if (y == 'Y' || y == 'y'){
			break;
		}
		cout << endl;
	}

	cout << "Final Matrix 2" << endl;
	matrix2->printMatrix();
	cout << endl;
	cout << endl;
	cout << endl;

	cout << "Result Adding Matrix 1 and Matrix 2" << endl;
	try{
		matrix1->addSM(matrix2);
	}
	catch (exception e){ }
	
	cout << endl;
	cout << endl;
	int x;
	cin >> x;
};