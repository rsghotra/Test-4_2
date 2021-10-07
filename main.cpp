#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
using namespace std;

struct ListNode {
  ListNode* next;
  int data;
  ListNode(int data) {
    this->data = data;
    this->next  = 0;
  }
};

class Job {
  public:
    int start = 0;
    int end = 0;
    int load = 0;
    Job(int s, int e, int load) {
      this->start  = s;
      this->end = e;
      this->load = load;
    }
};

vector<int> FindAllAnagramsStartIndex(const string& str, const string& pattern) {
  vector<int> result;
  unordered_map<char, int> frequencies;
  for(char ch: pattern) {
    frequencies[ch]++;
  }

  int windowStart = 0;
  int matchCount = 0;
  for(int windowEnd = 0; windowEnd < str.size(); windowEnd++) {
    if(frequencies.find(str[windowEnd]) != frequencies.end()) {
      frequencies[str[windowEnd]]--;
      if(frequencies[str[windowEnd]] == 0) {
        matchCount++;
      }
    }
    if(matchCount == frequencies.size()) {
      result.push_back(windowStart);
    }

    if(windowEnd - windowStart + 1 == pattern.size()) {
      if(frequencies.find(str[windowStart]) != frequencies.end()) {
        if(frequencies[str[windowStart]] == 0) {
          matchCount--;
        }
        frequencies[str[windowStart]]++;
      }
      windowStart++;
    }
  }
  return result;
}

void FindAllAnagramsStartIndex() {
  cout << "Looking for Anagrams" << endl;

  vector<int> result = FindAllAnagramsStartIndex("abbcabc", "abc");
  cout << "Starting index found to be: " << endl;
  for(auto index: result) {
    cout << index << " ";
  }
  cout << endl;
}

int nextIndex(const string& str1, int index) {
  int backspace = 0;
  while(index >= 0) {
    if(str1[index] == '#') {
      backspace++;
    } else if(backspace > 0) {
      backspace--;
    } else {
      break;
    }
    index--;
  }
  return index;
}

bool BackspaceCompare(const string& str1, const string& str2) {
  int ptr1 = str1.size() - 1;
  int ptr2 = str2.size() - 1;

  //interesting ||
  while(ptr1 >= 0 || ptr2 >= 0) {
    int I1 = nextIndex(str1, ptr1);
    int I2 = nextIndex(str2, ptr2);

    if(I1 < 0 && I2 < 0) {
      return true;
    }
    if(I1 < 0 || I2 < 0) {
      return false;
    }

    if(str1[I1] != str2[I2]) {
      return false;
    }
    ptr1 = --I1;
    ptr2 = --I2;
  }
  return true;
}

void BackspaceCompare() {
  cout << "Backspace compare" << endl;
  cout << BackspaceCompare("xy#z", "xzz#") << endl;
  cout << BackspaceCompare("xy#z", "xyz#") << endl;
  cout << BackspaceCompare("xp#", "xyz##") << endl;
  cout << BackspaceCompare("xywrrmp", "xywrrmu#p") << endl;
}

ListNode* Reverse(ListNode* head) {
  ListNode* previous = 0;
  ListNode* current = head;

  while(current != 0) {
    ListNode* temp = current->next;
    current->next = previous;
    previous = current;
    current = temp;
  }

  return previous;
}

ListNode* ReverseAlternativeLL(ListNode* head) {
  // find the mid;
  ListNode* slow = head;
  ListNode* fast = head;

  while(fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
  }
  ListNode* headFirst = head;
  ListNode* headSecond = Reverse(slow);

  while(headFirst != 0 && headSecond != 0) {
    ListNode* temp = headFirst->next;
    headFirst->next = headSecond;
    headFirst = temp;

    temp = headSecond->next;
    headSecond->next = headFirst;
    headSecond = temp;
  }
  if(headFirst != 0) {
    headFirst->next = 0;
  }
  return headFirst;
}

void ReverseAlternativeLL() {
  ListNode *head = new ListNode(2);
  head->next = new ListNode(4);
  head->next->next = new ListNode(6);
  head->next->next->next = new ListNode(8);
  head->next->next->next->next = new ListNode(10);
  head->next->next->next->next->next = new ListNode(12);
  ReverseAlternativeLL(head);
  while (head != nullptr) {
    cout << head->data << " ";
    head = head->next;
  }
}

ListNode* ReverseEveryKSizedLL(ListNode* head, int K) {
  ListNode* previous = 0;
  ListNode* current = head;

  while(current != 0) {
    ListNode* lastPrevious = previous;
    ListNode* lastCurrent = current;

    for(int i = 0; i < K && current != 0; i++) {
      ListNode* temp = current->next;
      current->next = previous;
      previous = current;
      current = temp;
    }

    if(lastPrevious != 0) {
      lastPrevious->next = previous;
    } else {
      head = previous;
    }
    lastCurrent->next = current;
    if(current == 0){
      break;
    }
    //re-setting previous; to lastCurrent as it will now be the last node of the reversed sublist
    previous = lastCurrent;
  }
  return head;
}

void ReverseEveryKSizedLL() {
  ListNode *head = new ListNode(1);
  head->next = new ListNode(2);
  head->next->next = new ListNode(3);
  head->next->next->next = new ListNode(4);
  head->next->next->next->next = new ListNode(5);
  head->next->next->next->next->next = new ListNode(6);
  head->next->next->next->next->next->next = new ListNode(7);
  head->next->next->next->next->next->next->next = new ListNode(8);

  ListNode *result = ReverseEveryKSizedLL(head, 3);
  cout << "\nNodes of the reversed LinkedList are: ";
  while (result != nullptr) {
    cout << result->data << " ";
    result = result->next;
  }
}

struct EndCompare {
  bool operator()(const Job& x, const Job& y) {
    return x.end > y.end;
  }
};


int MaxCPULoad(vector<Job>& jobs) {
  //sort the jobs by start time
  priority_queue<Job, vector<Job>, EndCompare> minHeap;
  std::sort(jobs.begin(), jobs.end(), [](const Job& x, const Job& y) {
    return x.start < y.start;
  });
  int maxLoad = 0;
  int currentLoad = 0;
  for(auto job: jobs) {
    while(!minHeap.empty() && job.start > minHeap.top().end) {
      currentLoad -= minHeap.top().load;
      minHeap.pop();
    }
    minHeap.push(job);
    currentLoad += job.load;
    maxLoad = max(currentLoad, maxLoad);
  }
  return maxLoad;
}


void  MaxCPULoad() {
  vector<Job> input = {{1, 4, 3}, {7, 9, 6}, {2, 5, 4}};
  cout << "\nMaximum CPU load at any time: " << MaxCPULoad(input) << endl;

  input = {{6, 7, 10}, {8, 12, 15}, {2, 4, 11}};
  cout << "Maximum CPU load at any time: " << MaxCPULoad(input) << endl;

  input = {{1, 4, 2}, {3, 6, 5}, {2, 4, 1}};
  cout << "Maximum CPU load at any time: " << MaxCPULoad(input) << endl;
}


int main() {
  FindAllAnagramsStartIndex();
  BackspaceCompare();
  ReverseAlternativeLL();
  MaxCPULoad();
  ReverseEveryKSizedLL();
  return 0;
}