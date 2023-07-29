#include <iostream>
#include <iomanip>
#include <string>
#include <string.h>
#include <sapi.h>
#include <windows.h>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <conio.h>
#include <sstream>
#include <cstdio>
#include <fileapi.h>

using namespace std;

int globalcount = 0;
int vflag = 0;
int cflag = 0;

class Node
{
    string data;
    string text;
    Node* left;
    Node* right;
    int height;
    friend class AVLTree;

public:

    Node()
    {
        data = "";
        left = NULL;
        right = NULL;
        height = 1;
        text = "";
    }
};

class AVLTree
{
    Node* root;
public:
    AVLTree()
    {
        root = NULL;
    }
    Node* getRoot()
    {
        return root;
    }
    void InOrder()
    {
        In_Order(root);
    }
    void In_Order(Node* leaf)
    {
        if (leaf != NULL)
        {
            In_Order(leaf->left);
            cout << leaf->data << "\t";
            cout << leaf->text;
            cout << endl;
            In_Order(leaf->right);
        }
    }
    void PostOrder()
    {
        Post_Order(root);
    }
    void Post_Order(Node* leaf)
    {
        if (leaf != NULL)
        {
            Post_Order(leaf->left);
            Post_Order(leaf->right);
            cout << leaf->data << "\t";
            cout << leaf->text;
            cout << endl;
        }
    }
    void PreOrder()
    {
        Pre_Order(root);
    }
    void Pre_Order(Node* leaf)
    {
        if (leaf != NULL)
        {
            cout << leaf->data << "\t";
            cout << leaf->text;
            cout << endl;
            Pre_Order(leaf->left);
            Pre_Order(leaf->right);
        }
    }
    int CountNodes()
    {
        return Count_Nodes(root);
    }
    int Count_Nodes(Node* leaf)
    {
        if (leaf == NULL)
        {
            return 0;
        }
        else
        {
            int count = 1;
            count += Count_Nodes(leaf->left);
            count += Count_Nodes(leaf->right);
            return count;
        }
    }
    int MaximumHeight()
    {
        return Maximum_Height(root);
    }
    int Maximum_Height(Node* leaf)
    {
        if (leaf == NULL)
        {
            return -1;
        }
        else
        {
            int leftHeight = Maximum_Height(leaf->left);
            int rightHeight = Maximum_Height(leaf->right);
            if (leftHeight > rightHeight)
            {
                return leftHeight + 1;
            }
            else
            {
                return rightHeight + 1;
            }
        }
    }
    int Maximum(int a, int b)
    {
        return (a > b) ? a : b;
    }
    int Height(Node* n)
    {
        if (n == NULL)
        {
            return 0;
        }
        return n->height;
    }
    Node* minValueNode(Node* node)
    {
        Node* current = node;
        while (current->left != NULL)
        {
            current = current->left;
        }
        return current;
    }
    Node* NewNode(string value1, string value2)
    {
        Node* leaf = new Node;
        leaf->data = value1;
        leaf->text = value2;
        leaf->left = NULL;
        leaf->right = NULL;
        leaf->height = 1;
        return(leaf);
    }
    Node* Insert(Node* Node, string value1, string value2)
    {
        if (Node == NULL)
        {
            return (NewNode(value1, value2));
        }
        if (value1 < Node->data)
        {
            Node->left = Insert(Node->left, value1, value2);
        }
        else
        {
            Node->right = Insert(Node->right, value1, value2);
        }
        Node->height = Maximum(Height(Node->left), Height(Node->right)) + 1;
        int balance = getBalance(Node);
        if (balance > 1 && value1 < Node->left->data)
            return rightRotate(Node);
        if (balance < -1 && value1 > Node->right->data)
            return leftRotate(Node);
        if (balance > 1 && value1 > Node->left->data)
        {
            Node->left = leftRotate(Node->left);
            return rightRotate(Node);
        }
        if (balance < -1 && value1 < Node->right->data)
        {
            Node->right = rightRotate(Node->right);
            return leftRotate(Node);
        }
        return Node;
    }
    Node* Delete(Node* leaf, string value)
    {
        if (leaf == NULL)
        {
            return leaf;
        }
        if (value < leaf->data)
        {
            leaf->left = Delete(leaf->left, value);
        }
        else if (value > leaf->data)
        {
            leaf->right = Delete(leaf->right, value);
        }
        else
        {
            if (leaf->left == NULL || leaf->right == NULL)
            {
                Node* temp;
                temp = leaf->left ? leaf->left : leaf->right;
                if (temp == NULL)
                {
                    temp = leaf;
                    leaf = NULL;
                }
                else
                {
                    *leaf = *temp;
                    free(temp);
                }
            }
            else
            {
                Node* temp = minValueNode(leaf->right);
                leaf->data = temp->data;
                leaf->right = Delete(leaf->right, temp->data);
            }

        }
        if (leaf == NULL)
        {
            return leaf;
        }
        leaf->height = 1 + Maximum(Height(leaf->left), Height(leaf->right));
        int balance = getBalance(leaf);
        if (balance > 1 && getBalance(leaf->left) >= 0)
        {
            return rightRotate(leaf);
        }
        if (balance > 1 && getBalance(leaf->left) < 0)
        {
            leaf->left = leftRotate(leaf->left);
            return rightRotate(leaf);
        }
        if (balance < -1 && getBalance(leaf->right) <= 0)
        {
            return leftRotate(leaf);
        }
        if (balance < -1 && getBalance(leaf->right) > 0)
        {
            leaf->right = rightRotate(leaf->right);
            return leftRotate(leaf);
        }
        return leaf;
    }
    Node* rightRotate(Node* y)
    {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = Maximum(Height(y->left), Height(y->right)) + 1;
        x->height = Maximum(Height(x->left), Height(x->right)) + 1;
        return x;
    }
    Node* leftRotate(Node* x)
    {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = Maximum(Height(x->left), Height(x->right)) + 1;
        y->height = Maximum(Height(y->left), Height(y->right)) + 1;
        return y;
    }
    int getBalance(Node* N)
    {
        if (N == NULL)
            return 0;
        return Height(N->left) - Height(N->right);
    }

    void KeyWords()
    {
        string main;
        string output1;
        string output2;
        fstream file;
        size_t len;
        int flag = 0, i;
        file.open("Keywords.txt", ios::in);
        while (getline(file, main))
        {
            flag = 0;
            output1 = "";
            output2 = "";
            for (i = 0, len = main.length(); i < len; i++)
            {
                if (main[i] == ',')
                {
                    main.erase(i--, 1);
                    len = main.length();
                }
                else
                {
                    if (main[i] == '|' || flag == 1)
                    {
                        if (flag == 0)
                        {
                            i++;
                        }
                        output2 += main[i];
                        flag = 1;
                    }
                    else if (flag == 0)
                    {
                        output1 += main[i];
                    }
                }
            }
            root = Insert(root, output1, output2);
        }
        file.close();
        return;
    }
    Node* Search(string value, Node* leaf)
    {
        if (leaf != NULL)
        {
            if (value == leaf->data)
            {
                return leaf;
            }
            else if (value < leaf->data)
            {
                return Search(value, leaf->left);
            }
            else if (value >= leaf->data)
            {
                return Search(value, leaf->right);
            }
            return NULL;
        }
        else
        {
            globalcount += 1;
            return NULL;
        }
    }
    void BinarySearch(int wordcounter, string value, vector <string> vect)
    {
        wstring input;
        char in;
        ISpVoice* pVoice = NULL;
        if (FAILED(CoInitialize(NULL)))
        {
            return;
        }
        HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
        Node* temp = new Node();
        temp = Search(value, root);
        if (temp != NULL)
        {
            //cout << "Found" << endl;
            //cout << temp->text;
            if (cflag == 1)
            {
                cout << "There is another smiliar response available for your query. Would you like to see that? (Y/N)\n";
                hr = pVoice->Speak(L"There is another smiliar response available for your query. Would you like to see that?", 0, NULL);
                cin >> in;
                if (in == 'Y' || in == 'y')
                {

                }
                else
                {
                    goto skip;
                }
            }
            KnownResponses(temp->data, temp->text);
        }
        else if (temp == NULL)
        {
            //cout << "Not found." << endl;
            if (globalcount == wordcounter)
            {
                UnknownResponses(vect);
            }
        }
    skip:
        cout << "";
    }
    void KnownResponses(string value1, string value2)
    {
    again:
        ISpVoice* pVoice = NULL;
        if (FAILED(CoInitialize(NULL)))
        {
            return;
        }
        HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
        string response, temp, read;
        int i = 0, count = 0;
        fstream file;
        fstream file1;
        cout << endl;
        string filename = value1;
        filename += "ratings.txt";
        string conversion;
        wstring wstr1;
        file.open(value2.c_str(), ios::in);
        if (value2 == "DialogueUnknown.txt")
        {
            file1.open(filename.c_str(), ios::in);
            while (getline(file1, read))
            {
                count++;
            }
            if (count >= 9)
            {
                CalculateSum(value1);
                file1.close();
                file.close();
                value2 = "DialogueKnown.txt";
                goto again;
            }
            else
            {
                Ratings(value1);
            }
        }
        else if (value2 == "DialogueFees.txt")
        {
            getline(file, response);
            if (response[i] == '(')
            {
                while (response[i] != ')')
                {
                    i++;
                }
                i++;
                for (i; i < response.length(); i++)
                {

                    cout << response[i];
                }
                hr = pVoice->Speak(L"Annual fees is approximately between 150,000 and 200,000 rupees", 0, NULL);
            }
            cout << "\nThere are many more factors that go into fee calculation.\nWould you like to see the structure broken down into more detail? (Y/N): ";
            hr = pVoice->Speak(L"There are many more factors that go into fee calculation, Would you like to see the structure broken down into more detail?", 0, NULL);
            string c;
            while (1)
            {
                getline(cin, c);
                if (c == "Y" || c == "y")
                {
                    fstream fee;
                    string foutput;
                    cout << "\n                ADMISSION DETAILS                    \n";
                    cout << "-----------------------------------------------------\n";
                    fee.open("FeeStructure.txt", ios::in);
                    while (getline(fee, foutput))
                    {
                        cout << foutput << endl;
                    }
                    cout << "Alternatively, you can also visit the website for a more concise format.\nWould you like to go there instead?(Y/N)";
                    hr = pVoice->Speak(L"Alternatively, you can also visit the website for a more concise format, Would you like to go there instead?", 0, NULL);
                    while (1)
                    {
                        getline(cin, c);
                        if (c == "Y" || c == "y")
                        {
                            system("START https://nu.edu.pk/Admissions/FeeStructure");
                            return;
                        }
                        else if (c == "N" || c == "n")
                        {
                            cout << "Very well.\n";
                            hr = pVoice->Speak(L"Very well", 0, NULL);
                            return;
                        }
                        else
                        {
                            cout << "Incorrect choice,choose again.\n";
                            hr = pVoice->Speak(L"Incorrect choice,choose again", 0, NULL);
                        }

                    }
                    break;
                }
                else if (c == "N" || c == "n")
                {
                    cout << "Very well.\n";
                    hr = pVoice->Speak(L"Very well", 0, NULL);
                    return;
                }
                else
                {
                    cout << "Incorrect choice, choose again.\n";
                    hr = pVoice->Speak(L"Incorrect choice,choose again", 0, NULL);
                }
            }

        }
        else
        {
            while (getline(file, response))
            {
                if (strstr(response.c_str(), value1.c_str()))
                {
                    if (response[i] == '(')
                    {
                        while (response[i] != ')')
                        {
                            i++;
                        }
                        i++;
                        for (i; i < response.length(); i++)
                        {
                            conversion += response[i];
                            cout << response[i];
                        }
                        wstr1 = wstring(conversion.begin(), conversion.end());
                        hr = pVoice->Speak(wstr1.c_str(), 0, NULL);
                        conversion.clear();
                        cout << endl;
                        break;
                    }
                    else
                    {
                        conversion += response;
                        cout << response;
                        wstr1 = wstring(conversion.begin(), conversion.end());
                        hr = pVoice->Speak(wstr1.c_str(), 0, NULL);
                        conversion.clear();
                        cout << endl;
                        break;
                    }
                }


            }
            return;
        }
    }
    void UnknownResponses(vector <string> vect)
    {
        wstring input;
        ISpVoice* pVoice = NULL;
        if (FAILED(CoInitialize(NULL)))
        {
            return;
        }
        HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
        vector <string> arr;
        arr = IgnoreCommonWords(vect);
        int y = 0;
        string response;
        fstream file;
        cout << "I am currently unable to answer this question. Please add a response you believe to be appropriate." << endl;
        hr = pVoice->Speak(L"I am currently unable to answer this question. Please add a response you believe to be appropriate.", 0, NULL);
        while (y != 3)
        {
            cout << "\nEnter your response (" << y + 1;
            cout << "/3): ";
            hr = pVoice->Speak(L"Enter your response.", 0, NULL);
            getline(cin, response);
            file.open("DialogueUnknown.txt", ios::app);
            file << "(";
            for (int i = 0; i < arr.size(); i++)
            {
                if (arr[i] != "@")
                {
                    file << " ";
                    file << arr[i];
                    file << " ";
                    //file << ",";
                }
            }
            file << ")";
            file << response;
            file << endl;
            file.close();
            y++;

        }
        cout << "Thank you for your valuable responses\n";
        hr = pVoice->Speak(L"Thank you for your valuable responses", 0, NULL);
        pVoice->Release();
        pVoice = NULL;
    }
    vector <string> IgnoreCommonWords(vector <string> vect)
    {
        int i, j;
        string commons[] = { "there","best","u","an","a","are","for","you","it","i","am","and","or","not","is","if","to","be","of","in","at","from","with","about","between","above","below","because","after","such","as","he","she","they","them","the","me","myself","by","students","his","her","him","its","our","who","what","how","why","when","your","mine" };
        int len = sizeof(commons) / sizeof(commons[0]);
        for (i = 0; i < vect.size(); i++)
        {
            for (j = 0; j < len; j++)
            {
                if (vect[i] == commons[j])
                {
                    vect[i] = "@";
                }
            }
        }
        fstream file;
        file.open("Keywords.txt", ios::app);

        for (i = 0; i < vect.size(); i++)
        {
            if (vect[i] != "@")
            {
                file << "\n";
                file << vect[i];
                file << "|DialogueUnknown.txt";
            }
        }
        file.close();
        return vect;
    }
    void Ratings(string key)
    {

        wstring input;
        ISpVoice* pVoice = NULL;
        if (FAILED(CoInitialize(NULL)))
        {
            return;
        }
        HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
        fstream file;
        fstream temp;
        string filename = key;
        filename += "ratings.txt";
        string response;
        int rating;
        int i = 0, j = 0;
        int count = 0;
        //cout << "Keyword : " << key << endl;
        file.open("DialogueUnknown.txt", ios::in);
        temp.open(filename.c_str(), ios::app);
        string conversion;
        wstring wstr1, wstr2, wstr3;
        while (getline(file, response))
        {
            if (strstr(response.c_str(), key.c_str()))
            {
                i = 0;
                if (response[i] == '(')
                {
                    while (response[i] != ')')
                    {
                        i++;
                    }
                    i++;
                    for (i; i < response.length(); i++)
                    {
                        conversion += response[i];
                        cout << response[i];
                    }
                    wstr1 = wstring(conversion.begin(), conversion.end());
                    hr = pVoice->Speak(wstr1.c_str(), 0, NULL);
                    cout << "\nThis is an experimental response, please rate it on a scale of 1-10\n->";
                    hr = pVoice->Speak(L"This is an experimental response, please rate it on a scale of 1-10", 0, NULL);
                    while (1)
                    {
                        cin >> rating;
                        if (rating > 10 || rating < 0)
                        {
                            cout << "Try again, a rating must be between 1-10\n->";
                        }
                        else
                        {
                            break;
                        }
                    }
                    temp << rating << endl;
                    getline(file, response);
                    i = 0;
                    conversion.clear();
                    if (response[i] == '(')
                    {
                        while (response[i] != ')')
                        {
                            i++;
                        }
                        i++;
                        for (i; i < response.length(); i++)
                        {
                            conversion += response[i];
                            cout << response[i];
                        }
                        wstr2 = wstring(conversion.begin(), conversion.end());
                        hr = pVoice->Speak(wstr2.c_str(), 0, NULL);
                        cout << "\nThis is an experimental response, please rate it on a scale of 1-10\n->";
                        hr = pVoice->Speak(L"This is an experimental response, please rate it on a scale of 1-10", 0, NULL);
                        while (1)
                        {
                            cin >> rating;
                            if (rating > 10 || rating < 0)
                            {
                                cout << "Try again, a rating must be between 1-10\n->";
                            }
                            else
                            {
                                break;
                            }
                        }
                        temp << rating << endl;
                        getline(file, response);
                        i = 0;
                        conversion.clear();
                        if (response[i] == '(')
                        {
                            while (response[i] != ')')
                            {
                                i++;
                            }
                            i++;
                            for (i; i < response.length(); i++)
                            {
                                conversion += response[i];
                                cout << response[i];
                            }
                            wstr3 = wstring(conversion.begin(), conversion.end());
                            hr = pVoice->Speak(wstr3.c_str(), 0, NULL);
                            cout << "\nThis is an experimental response, please rate it on a scale of 1-10\n->";
                            hr = pVoice->Speak(L"This is an experimental response, please rate it on a scale of 1-10", 0, NULL);
                            while (1)
                            {
                                cin >> rating;
                                if (rating > 10 || rating < 0)
                                {
                                    cout << "Try again, a rating must be between 1-10\n->";
                                }
                                else
                                {
                                    break;
                                }
                            }
                            temp << rating << endl;
                            temp.close();
                            break;
                        }
                    }
                }
            }
        }
        file.close();
        return;
    }
    void CalculateSum(string value1)
    {
        fstream file;
        int sum1 = 0, sum2 = 0, sum3 = 0;
        int var = 0, count = 1;
        string read = "";
        string filename = value1;
        filename += "ratings.txt";
        file.open(filename.c_str(), ios::in);
        while (getline(file, read))
        {
            if (count == 1 || count == 4 || count == 7)
            {
                stringstream(read) >> var;
                sum1 += var;
                var = 0;
            }
            else if (count == 2 || count == 5 || count == 8)
            {
                stringstream(read) >> var;
                sum2 += var;
                var = 0;
            }
            else if (count == 3 || count == 6 || count == 9)
            {
                stringstream(read) >> var;
                sum3 += var;
                var = 0;
            }
            count++;
        }
        file.close();
        //cout << "Sum 1 : " << sum1 <<endl;
        //cout << "Sum 2 : " << sum2 << endl;
        //cout << "Sum 3 : " << sum3 << endl;
        int max1 = max(sum1, sum2);
        int max2 = max(max1, sum3);
        int max = max(max1, max2);
        //cout << "Max rating : " << max << endl;
        if (max == sum1)
        {
            AddLearnedResponse(1, value1);
        }
        else if (max == sum2)
        {
            AddLearnedResponse(2, value1);
        }
        else
        {
            AddLearnedResponse(3, value1);
        }
    }
    void AddLearnedResponse(int x, string keyword)
    {
        fstream file;
        string read, temp;
        int flag = 0, linecount = 0;
        int i = 0;
        file.open("DialogueUnknown.txt", ios::in);
        while (getline(file, read))
        {
            //temp.clear();
            linecount++;
            if (strstr(read.c_str(), keyword.c_str()))
            {
                i = 0;
                if (read[i] == '(')
                {
                    while (read[i] != ')')
                    {
                        i++;
                    }
                    i++;
                    if (x == 1)
                    {
                        for (i; i < read.length(); i++)
                        {
                            temp += read[i];
                        }
                        break;
                    }
                    else if (x == 2)
                    {
                        getline(file, read);
                        for (i; i < read.length(); i++)
                        {
                            temp += read[i];
                        }
                        break;
                    }
                    else if (x == 3)
                    {
                        getline(file, read);
                        getline(file, read);
                        for (i; i < read.length(); i++)
                        {
                            temp += read[i];
                        }
                        break;
                    }
                }
            }
        }
        //cout << temp;
        file.close();
        file.open("DialogueKnown.txt", ios::app);
        file << "( " << keyword << " )";
        file << temp << endl;
        file.close();
        UpdateKeyword(keyword);
    }
    void UpdateKeyword(string keyword)
    {
        string temp = keyword;
        string read;
        temp += "|DialogueUnknown.txt";
        fstream file;
        fstream fileptr;
        string filename = keyword;
        filename += "ratings.txt";
        file.open("Keywords.txt", ios::in);
        fileptr.open("Temp.txt", ios::out);
        while (getline(file, read))
        {
            if (read != temp)
            {
                fileptr << read << endl;
            }
        }
        file.close();
        fileptr.close();
        remove("Keywords.txt");
        int r = rename("Temp.txt", "Keywords.txt");
        keyword += "|DialogueKnown.txt";
        file.open("Keywords.txt", ios::app);
        file << keyword << endl;
        file.close();
        remove(filename.c_str());
        //KeyWords();
    }
    void AdminMode()
    {
        string pin;
        cout << "Enter the required PIN: \n->";
        cin >> pin;
        while (pin != "XXXX")
        {
            if (pin == "XXXX")
            {
                cout << "Access granted\n";
            }
            if (pin == "Q" || pin == "q")
            {
                return;
            }
            else
            {
                cout << "Incorrect PIN, try again\n";
            }
        }
        cout << "Admin mode has been activated. \nYou have been granted special privileges and limited functionality.\n";
        string input, deletethis;
        do
        {
            input.clear();
            cout << "Enter your input(Q to exit): ";
            cin.ignore();
            getline(cin, input);
            if (input == "Q" || input == "q")
            {
                break;
            }
            if (strstr(input.c_str(), "exit") || strstr(input.c_str(), "Exit") || strstr(input.c_str(), "quit") || strstr(input.c_str(), "Quit"))
            {
                break;
            }
            if ((strstr(input.c_str(), "delete")) || (strstr(input.c_str(), "Delete")))
            {
                cout << "Enter a keyword to be deleted: ";
                getline(cin, deletethis);
                root = Delete(root, deletethis);
                DeleteKeyword(deletethis);
                KeyWords();

            }
        } while (1);
        cout << "Exiting admin mode...";
    }
    void DeleteKeyword(string keyword)
    {
        fstream fileptr;
        fstream temp;
        string read;
        fileptr.open("Keywords.txt", ios::in);
        temp.open("TempKeywords.txt", ios::out);
        while (getline(fileptr, read))
        {
            if (!strstr(read.c_str(), keyword.c_str()))
            {
                temp << read << endl;
            }
        }
        fileptr.close();
        temp.close();
        remove("Keywords.txt");
        int x = rename("TempKeywords.txt", "Keywords.txt");
    }
};

void Display()
{
    string output;
    fstream file;
    file.open("Formatting.txt", ios::in);
    while (getline(file, output))
    {
        cout << output << endl;
    }
    file.close();
}

void Driver()
{
    AVLTree T;

repeat:
    T.KeyWords();
    ISpVoice* pVoice = NULL;
    if (FAILED(CoInitialize(NULL)))
    {
        return;
    }
    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
    globalcount = 0;
    fflush(stdin);
    string search, tempword;
    vector <string> arr;
    int wordcounter = 0, i;
    size_t len;
    Display();
    if (vflag == 0)
    {
        hr = pVoice->Speak(L"Chat bot hero welcomes you to FAST Admission FAQ Portal", 0, NULL);
        vflag = 1;
    }
    cout << "What would you like to say?" << endl;
    hr = pVoice->Speak(L"What would you like to say?", 0, NULL);
    getline(cin, search);
    //cout << search << " " << search.length() << endl;
    search += " ";
    for (i = 0, len = search.length(); i < len; i++)
    {
        if (ispunct(search[i]))
        {
            search.erase(i--, 1);
            len = search.length();
        }
        else if (search[i] == ' ')
        {
            //cout << tempword << endl;
            wordcounter = wordcounter + 1;
            arr.push_back(tempword);
            tempword = "";
        }
        else
        {
            if (isalpha(search[i]))
            {
                if (isupper(search[i]))
                {
                    search[i] += 32;
                }
                else
                {

                }
                tempword += search[i];
            }
        }
    }
    //cout << wordcounter << endl;
    for (i = wordcounter - 1; i >= 0; i--)
    {
        //cout << arr[i] << endl;
        if (arr[i] == "leave" || arr[i] == "quit" || arr[i] == "exit" || arr[i] == "terminate")
        {
            goto gotohere;
        }
        else if (arr[i] == "admin")
        {
            T.AdminMode();
            goto skipsearch;
        }
        T.BinarySearch(wordcounter, arr[i], arr);
    }
skipsearch:

    cout << "\nPress any key to continue." << endl;
    hr = pVoice->Speak(L"Press any key to continue", 0, NULL);
    char c;
    c = _getch();
    system("cls");

    goto repeat;
gotohere:

    cout << "It was a pleasure to help you, thank you for using our Admission portal." << endl;
    cout << "Have a great day, HiRo says goodbye!";
    hr = pVoice->Speak(L"E:\\VSCode\\Data Structures\\Project\\Project\\DialogueLeaving.txt", SPF_IS_FILENAME, NULL);
    return;
}

bool OptionalVoice()
{
    wstring input;

    ISpVoice* pVoice = NULL;

    if (FAILED(CoInitialize(NULL)))
    {
        return FALSE;
    }

    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);

    if (SUCCEEDED(hr))
    {
        //cout << "Enter text: ";
        //getline(wcin, input);
        //hr = pVoice->Speak(input.c_str(), 0, NULL);
        hr = pVoice->Speak(L"E:\\VSCode\\Data Structures\\Project\\Project\\Formatting.txt", SPF_IS_FILENAME, NULL);
        //hr = pVoice->Speak(L"This sounds normal but the pitch drops half way through", 0, NULL);
        //hr = pVoice->Speak(L"D:\\TTS.txt", SPF_IS_FILENAME, NULL);
        //hr = pVoice->Speak(L"This sounds normal <pitch absmiddle = '-10'/> but the pitch drops half way through", SPF_IS_XML, NULL);
        //hr = pVoice->Speak(L"I don't think my <pitch middle = '-15'/> creators will allow me that.", SPF_IS_XML, NULL);
        pVoice->Release();
        pVoice = NULL;
    }
    CoUninitialize();
    return TRUE;
}

int main()
{
    Driver();
}
