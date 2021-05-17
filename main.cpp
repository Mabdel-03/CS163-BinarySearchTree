/* Code written by Mahmoud Abdelmoneum
 * Code written on 5/15/2021
 * The Binary Search Tree program utilizes a BST to hold numbers 
 * between 1 and 999
 * Users can add, remove, and print
 */
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;

struct Node {
  Node* left;
  Node* right;
  int value;
};

//function prototypes ************
void quit();
bool isCommandValid(char command[10]);
void getManualNums(Node* &bst);
bool checkRange(int num);
void openFile(Node* &bst);
void addNode(Node* &bst, int num); 
void print(Node *bst, int depth);
void deleteNode(Node* &bst, int num);
void searchNode(Node* bst, int num);
Node* findSmallest(Node* bst);
//a function to print out a cute ascii tree (didn't want to
//study for bio so did this instead)
void asciiTree(); 
//********************************

//finds the smallest num (most left) in the right side of tree
Node* findSmallest(Node* bst){
  Node* traverse = bst;
  while(traverse != NULL && traverse->left != NULL){
    traverse = traverse->left;
  }
  //out of while loop, found node
  cout << "found it: " << traverse->value << endl;
  return traverse;
}

//deletes the node passed in
void deleteNode(Node* &bst, int num){
  cout << "bst value: " << bst->value << endl;
  if(bst == NULL){
    cout << "That number is not in the tree" << endl;
    return;
  }
  //FIRST: find where the node is using recursion
  if(num < bst->value){ // left
    cout << "traversing left. Currently at " << bst->left->value << endl;
    deleteNode(bst->left, num);
  }
  else if(num > bst->value){ //right
    cout << "traversing right. Currently at " << bst->right->value << endl;
    deleteNode(bst->right, num);
  }
  else { //found node to be deleted (num == bst->value)
    cout << "found value. num = " << num << " bst->value = "<< bst->value << endl;
    //case 1: node is a leaf - just remove
    if(bst->right == NULL && bst->left == NULL){
      cout << "leaf " << endl;
      delete(bst);
      bst = NULL;
    }
    //case 2: node has 1 child - copy child to node, delete child
    else if(bst->right == NULL && bst->left != NULL){ //only has left
      cout << "has left child only" << endl;
      Node* temp = bst;
      bst = bst->left;
      delete(temp);
      temp = NULL;
    }
    else if(bst->left == NULL && bst->right != NULL){ //only has right
      cout << "has right child only" << endl;
      Node* temp = bst;
      bst = bst->right; //copy child
      delete(temp);
      temp = NULL;
    }
    
    //case 3: node has 2 children - get smallest num in the right side
    //copy that to node to be deleted, delete the original
    else {
      cout << "has 2 children. great. " << endl;
      Node* temp = findSmallest(bst->right);
      int tempvalue = temp->value; //save original data
      deleteNode(bst, temp->value);
      bst->value = tempvalue; //copy over data
    }
  }
}

//search to see if a node is in the tree (based on passed in num)
void searchNode(Node* bst, int num){
  if(bst == NULL){ //nothing in tree, return null
    return;
  }
  if(num > bst->value){
    if(bst->right == NULL){ //if right child is null, then not there
      cout << "That number is NOT in the tree." << endl;
    } else { //keep traversing
      searchNode(bst->right, num);
    }
  }
  if(num < bst->value){ //same with left
    if(bst->left == NULL){
      cout << "That number is NOT in the tree." << endl;
    } else {
      searchNode(bst->left, num);
    }
  }
  if(num == bst->value){ //if found
    cout << "That number is in the tree." << endl;
  }
}

//uses the same print algorithm as Heap and Shunting Yard
void print(Node *bst, int depth){
  if(bst == NULL){
     return;
  }
  //print top child (right)
  print(bst->right, depth+1);
  //spacing                              
  for(int i = 0; i < depth; i++){
    cout << "    ";
  }
  cout << bst->value << endl;
  //bottom child(left)  
  print(bst->left, depth+1);
}

void addNode(Node* &bst, int num){
  //create head
  if(bst == NULL){
    bst = new Node;
    bst->value = num;
    bst->right = NULL;
    bst->left = NULL;
  }
  //create right
  if(num > bst->value){ 
    if(bst->right == NULL){
      Node* newnode = new Node;
      bst->right = newnode;
      newnode->right = NULL;
      newnode->left = NULL;
      newnode->value = num;
    } else {
      addNode(bst->right, num); //recursively call to find of right
    }	 
  }
  //create left
  else if(num < bst->value){ //find left
    if(bst->left == NULL){ //add it
      Node* newnode = new Node;
      bst->left = newnode;
      newnode->right = NULL;
      newnode->left = NULL;
      newnode->value = num;
    } else {
      addNode(bst->left, num);
    }
  }
}

//opens file and reads in numbers
void openFile(Node* &bst){
  ifstream inFile;
  int num = 0;
  char fileName[30];
  cout << "Remember: numbers must be separated by spaces. " << endl;
  cout << "What is the name of the file? " << endl;
  cin.get(fileName, 30);
  cin.get();
  inFile.open(fileName);
  
  if(!inFile){ //error out if cannot open
    cout << "Unable to open file. " << endl;
    cout << "Terminating program. Please re run and try again. " << endl; 
    exit(1);
  }
  while(inFile >> num){
    addNode(bst, num); //might need to be atoi??????????
  }
}

//checks range of numbers when inputted
bool checkRange(int num){
  if(num >= 1 && num <= 999){
    return true;
  }
  return false;
}

//console input
void getManualNums(Node* &bst){
  int num = 0;
  char input[1000];
  bool inRange = true;
  int count = 0;
  cout << "Adding manually." << endl;
  cout << "Enter up to 100 numbers with values between 1 and 999. " << endl;
  cout << "Remember to separate the numbers by spaces. " << endl;
  cin.get(input, 1000);
  cin.get();

  //tokenize (parse) input and check if in range
  char* token = strtok(input, " ");
  while(token != NULL){
    num = atoi(token);
    inRange = checkRange(num);
    if(inRange == true){
      addNode(bst, num); //if passed check, then send to addNode()
      token = strtok(NULL, " ");    
    } else { //not valid
      cout << "The value you entered was not in range." << endl;
      cout << "Terminating program. Please re run and try again. " << endl; 
      exit(0);
    }
  }
}

//checks that given command is valid
bool isCommandValid(char command[10]){
  if(strcmp(command, "ADD") == 0){
    return true;
  }
  if(strcmp(command, "REMOVE") == 0){
    return true;
  }
  if(strcmp(command, "PRINT") == 0){
    return true;
  }
  if(strcmp(command, "SEARCH") == 0){
    return true;
  }
  if(strcmp(command, "QUIT") == 0){
    return true;
  }
  return false;  
}

void quit(){
  cout << "Goodbye!" << endl;
}

void asciiTree(){
  cout << "          \\/ |    |/" << endl;
  cout << "       \\/ / \\||/  /_/___/_" << endl;
  cout << "        \\/   |/\\/" << endl;
  cout << "   _\\__\\_\\   |  /_____/_" << endl;
  cout << "          \\  | /          /" << endl;
  cout << "  __ _-----` |{,-----------~" << endl;
  cout << "           \\ }{" << endl;
  cout << "            }{{" << endl;
  cout << "            }}{" << endl;
  cout << "            {{}" << endl;
  cout << "      , -=-~{ .-^- _" << endl;
  cout << " " << endl;
  cout << "I am the powerful Binary Tree! " << endl;
  cout << "What number would you like to search?" << endl;

}

int main(){
  Node* bst = NULL; //tree to hold nodes
  int searchnum = 0;
  int deletenum = 0;
  char command[10];
  char manOrFile[10];
  bool isValid = false;
  int amountOfNums = 0;

  //welcome messages
  cout << "Welcome to Binary Search Tree!" << endl;
  while(strcmp(command, "QUIT") != 0){
    cout << "Enter a command: " << endl;
    cout << "(ADD/REMOVE/PRINT/SEARCH/QUIT)" << endl;
    cin.get(command, 10);
    cin.get();
    
    //make all uppercase
    for(int i = 0; i < 10; i++){ 
      command[i] = toupper(command[i]);
    }
    isValid = isCommandValid(command); //check validity
    if(isValid == true){
      if(strcmp(command, "ADD") == 0){
	cout << " " << endl;
	cout << "Would you like to: " << endl;
	cout << "(A) add manually " << endl << "(B) load from file" << endl;
	cin.get(manOrFile, 10);
	cin.get();
	
	//make uppercase again
	for(int j = 0; j < strlen(manOrFile); j++){ 
	  manOrFile[j] = toupper(manOrFile[j]);
	}
	if(strcmp(manOrFile, "A") == 0){ //get manual input
	  getManualNums(bst);
	  cout << "added." << endl;
	}
	else if(strcmp(manOrFile, "B") == 0){ //open file
	  openFile(bst);
	  cout << "added." << endl;
	}
	else {
	  cout << "That was not one of the options." << endl;
	  cout << " " << endl;
	}
      }
      //delete node
      if(strcmp(command, "REMOVE") == 0){
	cout << "What is the value of the node you would like to delete?" << endl;
	cin >> deletenum;
	cin.get();
	deleteNode(bst, deletenum);
      }
      //print out entire tree
      if(strcmp(command, "PRINT") == 0){
	print(bst, 0);
      }
      //search for a specific node
      if(strcmp(command, "SEARCH") == 0){
	if(bst == NULL){
	  cout << "There's nothing in the tree, so there's nothing to search!" << endl;
	  cout << " " << endl;
	} else {
	  asciiTree();
	  cin >> searchnum;
	  cin.get();
	  searchNode(bst, searchnum);
	  cout << " " << endl;
	}
      }
    }
  }
  quit();
  return 0;
}
