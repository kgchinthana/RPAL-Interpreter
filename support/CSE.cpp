#include "CSE.h"

CSE::CSE(Node *topNode)
{
    this->top = topNode;
}

CSE::CSE()
{
    // Constructor with no parameters
    numEnvironment = -1;
    PE = createNewEnvironment();
    environmentStack.push(PE);
    deltas = new vector<Ctrl *>;
    rootDelta = new Ctrl(Ctrl::DELTA, deltas->size());
}

CSE::~CSE()
{
    // Destructor
}

bool CSE::checkInbuilt(string funcName)
{
    // Function to check if a given function is an inbuilt function
    if (std::find(inbuiltFuncVector.begin(), inbuiltFuncVector.end(), funcName) != inbuiltFuncVector.end())
        return true;
    else
        return false;
}

void CSE::applyBinaryOPR(int type)
{
    // Function to apply binary operators like '+', '-', '*', '/', etc.
    // The function implementation is quite lengthy and deals with various binary operators.
    // It pops operands from the stack, performs the operation, and pushes the result back onto the stack.
    // The supported binary operators include '+', '-', '*', '/', '**', 'GR', 'GE', 'LS', 'LE', 'EQ', 'NE', 'AUG', 'OR', and 'AND_LOGICAL'.
    control.pop_back();
    Ctrl *rand1 = execStack.top();
    execStack.pop();
    Ctrl *rand2 = execStack.top();
    execStack.pop();
    Ctrl *result = new Ctrl();
    result->type = Ctrl::INTEGER;

    switch (type)
    {
    case Ctrl::AUG:
        result->type = Ctrl::TUPLE;
        result->ctrlTuples;
        if (rand1->type != Ctrl::NIL)
        {

            if (rand1->type == Ctrl::TUPLE)
            {
                for (int i = 0; i < rand1->ctrlTuples.size(); i++)
                    result->ctrlTuples.push_back(rand1->ctrlTuples.at(i));
            }
            else
            {
                result->ctrlTuples.push_back(rand1);
            }
        }

        if (rand2->type != Ctrl::NIL)
        {
            if (rand2->type == Ctrl::TUPLE)
            {
                for (int i = 0; i < rand2->ctrlTuples.size(); i++)
                    result->ctrlTuples.push_back(rand2->ctrlTuples.at(i));
            }
            else
                result->ctrlTuples.push_back(rand2);
        }
        break;

    case Ctrl::EXP:

        if (rand1->type == Ctrl::INTEGER && rand2->type == Ctrl::INTEGER)
        {

            result->ctrlVal = to_string(pow(atoi(rand1->ctrlVal.c_str()), atoi(rand2->ctrlVal.c_str())));
            break;
        }
        else
        {

            cout << "Incompatible arguments for '**'";
            exit(1);
        }

    case Ctrl::GR:
        if (rand1->type == Ctrl::INTEGER && rand2->type == Ctrl::INTEGER)
        {

            if (atoi(rand1->ctrlVal.c_str()) > atoi(rand2->ctrlVal.c_str()))
                result->type = Ctrl::TRUE;
            else
                result->type = Ctrl::FALSE;
            break;
        }

        else
        {

            cout << "Incompatible arguments for the operator 'GR'" << endl;
            exit(1);
        }

    case Ctrl::GE:

        if (rand1->type == Ctrl::INTEGER && rand2->type == Ctrl::INTEGER)
        {

            if (atoi(rand1->ctrlVal.c_str()) >= atoi(rand2->ctrlVal.c_str()))
                result->type = Ctrl::TRUE;
            else
                result->type = Ctrl::FALSE;
            break;
        }
        else
        {

            cout << "Incompatible arguments for the operator 'GE'";
            exit(1);
        }

    case Ctrl::LS:

        if (rand1->type == Ctrl::INTEGER && rand2->type == Ctrl::INTEGER)
        {

            if (atoi(rand1->ctrlVal.c_str()) < atoi(rand2->ctrlVal.c_str()))
                result->type = Ctrl::TRUE;
            else
                result->type = Ctrl::FALSE;
            break;
        }
        else
        {

            cout << "Incompatible arguments for the operator 'LS'";
            exit(1);
        }

    case Ctrl::LE:

        if (rand1->type == Ctrl::INTEGER && rand2->type == Ctrl::INTEGER)
        {

            if (atoi(rand1->ctrlVal.c_str()) <= atoi(rand2->ctrlVal.c_str()))
                result->type = Ctrl::TRUE;
            else
                result->type = Ctrl::FALSE;
            break;
        }
        else
        {

            cout << "Incompatible arguments for the operator 'LE'";
            exit(1);
        }

    case Ctrl::EQ:

        if (rand1->type == Ctrl::STRING && rand2->type == Ctrl::STRING)
        {
            if (rand1->ctrlVal.compare(rand2->ctrlVal) == 0)
                result->type = Ctrl::TRUE;
            else
                result->type = Ctrl::FALSE;
        }

        else if (rand1->type == Ctrl::INTEGER && rand2->type == Ctrl::INTEGER)
        {

            if (rand1->ctrlVal == rand2->ctrlVal)
                result->type = Ctrl::TRUE;
            else
                result->type = Ctrl::FALSE;
        }

        else if ((rand1->type == Ctrl::TRUE || rand1->type == Ctrl::FALSE) && (rand1->type == Ctrl::TRUE || rand2->type == Ctrl::FALSE))
            result->type = rand1->type == rand2->type ? Ctrl::TRUE : Ctrl::FALSE;

        else
        {

            cout << "Incompatible arguments for the operator 'EQ'";
            exit(1);
        }
        break;

    case Ctrl::NE:

        if (rand1->type == Ctrl::STRING && rand2->type == Ctrl::STRING)
        {

            if (rand1->ctrlVal.compare(rand2->ctrlVal) != 0)
                result->type = Ctrl::TRUE;
            else
                result->type = Ctrl::FALSE;
        }

        else if (rand1->type == Ctrl::INTEGER && rand2->type == Ctrl::INTEGER)
        {

            if (rand1->ctrlVal != rand2->ctrlVal)
                result->type = Ctrl::TRUE;
            else
                result->type = Ctrl::FALSE;
        }

        else if ((rand1->type == Ctrl::TRUE || rand1->type == Ctrl::FALSE) && (rand1->type == Ctrl::TRUE || rand2->type == Ctrl::FALSE))
            result->type = rand1->type != rand2->type ? Ctrl::TRUE : Ctrl::FALSE;

        else
        {

            cout << "Incompatible arguments for the operator 'NE'";
            exit(1);
        }
        break;

    case Ctrl::OR:

        if ((rand1->type == Ctrl::TRUE || rand1->type == Ctrl::FALSE) &&
            (rand2->type == Ctrl::TRUE || rand2->type == Ctrl::FALSE))
        {

            bool op1 = (rand1->type == Ctrl::TRUE) ? true : false;
            bool op2 = (rand2->type == Ctrl::TRUE) ? true : false;
            bool res = op1 | op2;
            result->type = res ? Ctrl::TRUE : Ctrl::FALSE;
        }
        else
        {

            cout << "Incompatible arguments for the operator 'OR'";
            exit(1);
        }
        break;

    case Ctrl::AND_LOGICAL:

        if ((rand1->type == Ctrl::TRUE || rand1->type == Ctrl::FALSE) &&
            (rand2->type == Ctrl::TRUE || rand2->type == Ctrl::FALSE))
        {

            bool op1 = (rand1->type == Ctrl::TRUE) ? true : false;
            bool op2 = (rand2->type == Ctrl::TRUE) ? true : false;
            bool res = op1 & op2;
            result->type = res ? Ctrl::TRUE : Ctrl::FALSE;
        }

        else
        {

            cout << "Incompatible arguments for the operator 'AND'";
            exit(1);
        }
        break;

    case Ctrl::ADD:

        if (rand1->type == Ctrl::INTEGER && rand2->type == Ctrl::INTEGER)
        {

            result->ctrlVal = to_string(atoi(rand1->ctrlVal.c_str()) + atoi(rand2->ctrlVal.c_str()));
            break;
        }

        else
        {

            cout << "Incompatible arguments for the operator '+'";
            exit(1);
        }

    case Ctrl::SUBTRACT:

        if (rand1->type == Ctrl::INTEGER && rand2->type == Ctrl::INTEGER)
        {

            result->ctrlVal = to_string(atoi(rand1->ctrlVal.c_str()) - atoi(rand2->ctrlVal.c_str()));
            break;
        }
        else
        {

            cout << "Incompatible arguments for the operator '-'";
            exit(1);
        }

    case Ctrl::MULTIPLY:

        if (rand1->type == Ctrl::INTEGER && rand2->type == Ctrl::INTEGER)
        {

            result->ctrlVal = to_string(atoi(rand1->ctrlVal.c_str()) * atoi(rand2->ctrlVal.c_str()));
            break;
        }
        else
        {

            cout << "Incompatible arguments for the operator '*'";
            exit(1);
        }

    case Ctrl::DIVIDE:

        if (rand1->type == Ctrl::INTEGER && rand2->type == Ctrl::INTEGER)
        {

            result->ctrlVal = to_string(atoi(rand1->ctrlVal.c_str()) / atoi(rand2->ctrlVal.c_str()));
            break;
        }
        else
        {

            cout << "Incompatible arguments for the operator '*'";
            exit(1);
        }

    default:
        cout << "UnHandled binary operator , type: " << type << endl;
    }

    execStack.push(result);
}

void CSE::escapePrintStr(string printStr)
{
    // Function to handle escape sequences and print a string with escapes properly.
    // The function iterates over the input string and handles escape sequences like '\n' (newline) and '\t' (tab).
    // It properly prints the characters with escapes using cout.

    for (int i = 0; i < printStr.length(); i++)
    {

        char ch1 = printStr.at(i);

        if (ch1 == '\\')
        {

            i++;

            if (i < printStr.length())
            {

                char ch2 = printStr.at(i);
                if (ch2 == 'n')
                    cout << endl;
                else if (ch2 == 't')
                    cout << "\t";
                else
                    cout << ch1 << ch2;
            }
        }
        else
            cout << ch1;
    }

    cout.flush();
}

void CSE::applyThisRator(Ctrl *rator)
{
    // Function to apply a given rator (operator) during the interpretation.

    // The function applies different operators like 'Print', 'Order', 'Isinteger', 'Istruthvalue', 'Isstring', 'Istuple', 'Isfunction', 'Isdummy', 'Stem', 'Stern', 'ItoS', 'Conc', 'Conclambda', 'Null', and others.
    // It performs the specific operation for each operator and pushes the result back onto the stack.

    Ctrl *temp;
    Ctrl *toPush;

    if (rator->variables.front() == "Print")
    {

        string print_str = execStack.top()->toStr();
        escapePrintStr(print_str); // WHY?
        execStack.pop();
        toPush = new Ctrl(Ctrl::DUMMY);
    }
    else if (rator->variables.front() == "Order")
    {

        Ctrl *tuple = execStack.top();
        execStack.pop();
        Ctrl *order = new Ctrl();
        order->type = Ctrl::INTEGER;

        if (tuple->type == Ctrl::TUPLE)
            order->ctrlVal = to_string(tuple->ctrlTuples.size());
        else if (tuple->type == Ctrl::NIL)
            order->ctrlVal = to_string(0);
        else
        {
            cout << "Invalid argument for 'Order'" << endl;
            exit(1);
        }
        toPush = order;
    }
    else if (rator->variables.front() == "Isinteger")
    {

        temp = execStack.top();
        execStack.pop();
        toPush = new Ctrl(temp->type == Ctrl::INTEGER ? Ctrl::TRUE : Ctrl::FALSE);
    }
    else if (rator->variables.front() == "Istruthvalue")
    {

        temp = execStack.top();
        execStack.pop();
        toPush = new Ctrl((temp->type == Ctrl::TRUE || temp->type == Ctrl::FALSE) ? Ctrl::TRUE : Ctrl::FALSE);
    }
    else if (rator->variables.front() == "Isstring")
    {

        temp = execStack.top();
        execStack.pop();
        toPush = new Ctrl(temp->type == Ctrl::STRING ? Ctrl::TRUE : Ctrl::FALSE);
    }
    else if (rator->variables.front() == "Istuple")
    {

        temp = execStack.top();
        execStack.pop();
        toPush = new Ctrl((temp->type == Ctrl::TUPLE || temp->type == Ctrl::NIL) ? Ctrl::TRUE : Ctrl::FALSE);
    }
    else if (rator->variables.front() == "Isfunction")
    {

        temp = execStack.top();
        execStack.pop();
        toPush = new Ctrl(temp->type == Ctrl::LAMBDA ? Ctrl::TRUE : Ctrl::FALSE);
    }
    else if (rator->variables.front() == "Isdummy")
    {

        temp = execStack.top();
        execStack.pop();
        toPush = new Ctrl(temp->type == Ctrl::DUMMY ? Ctrl::TRUE : Ctrl::FALSE);
    }
    else if (rator->variables.front() == "Stem")
    {

        if (execStack.top()->type == Ctrl::STRING)
        {
            Ctrl *strControl = new Ctrl(Ctrl::STRING);
            strControl->ctrlVal = execStack.top()->ctrlVal.substr(0, 1);
            execStack.pop();
            toPush = strControl;
        }
        else
        {
            cout << "STEM: Expecting String" << endl;
            exit(1);
        }
    }
    else if (rator->variables.front() == "Stern")
    {

        if (execStack.top()->type == Ctrl::STRING)
        {
            Ctrl *strControl = new Ctrl(Ctrl::STRING);
            strControl->ctrlVal = execStack.top()->ctrlVal.substr(1, execStack.top()->ctrlVal.length() - 1);
            execStack.pop();
            toPush = strControl;
        }
        else
        {
            cout << "STERN: Expecting String" << endl;
            exit(1);
        }
    }
    else if (rator->variables.front() == "ItoS")
    {

        if (execStack.top()->type == Ctrl::INTEGER)
        {
            Ctrl *strControl = new Ctrl(Ctrl::STRING);
            strControl->ctrlVal = execStack.top()->ctrlVal;
            execStack.pop();
            toPush = strControl;
        }
        else
        {
            cout << "Itos: Expecting Integer" << endl;
            exit(1);
        }
    }
    else if (rator->variables.front() == "Conc")
    {

        Ctrl *concl = new Ctrl(Ctrl::NAME);
        concl->variables.push_back("Conclambda");
        concl->variables.push_back(execStack.top()->ctrlVal);
        execStack.pop();
        toPush = concl;
    }
    else if (rator->variables.front() == "Conclambda")
    {
        Ctrl *concatVars = new Ctrl(Ctrl::STRING, rator->variables.at(1) + execStack.top()->ctrlVal);
        execStack.pop();
        toPush = concatVars;
    }
    else if (rator->variables.front() == "Null")
    {

        Ctrl *boolR = new Ctrl();
        if (execStack.top()->type == Ctrl::NIL || (execStack.top()->type == Ctrl::TUPLE && execStack.top()->ctrlTuples.empty()))
            boolR->type = Ctrl::TRUE;
        else
            boolR->type = Ctrl::FALSE;
        execStack.pop();
        toPush = boolR;
    }
    else
    {

        cout << "ERROR: value:%" << rator->ctrlVal << "%type:" << rator->type << endl;
        printCS();
        return;
    }
    execStack.push(toPush);
}

void CSE::handleName(Ctrl *temp, Ctrl *currControl, Ctrl *rator, Env *newEnv, int deltaIndex)
{
    // Function to handle the cases when a name is encountered during the interpretation.
    // It checks if the name refers to an inbuilt function or a variable in the current environment and handles accordingly.

    if (checkInbuilt(currControl->variables.front()))
    {
        control.pop_back();
        execStack.push(currControl);
    }
    else
    {
        temp = currEnvironment->lookup(currControl->variables.front());
        if (temp != NULL)
        {
            control.pop_back();
            execStack.push(temp);
        }
        else
        {
            cout << " Unknown name" + currControl->ctrlVal << endl;
        }
    }
}

void CSE::rule411(Ctrl *temp, Ctrl *currControl, Ctrl *rator, Env *newEnv, int deltaIndex)
{
    // Function to handle Rule 4 for interpreting lambda expressions.
    // The function creates a new environment, assigns variables to their values, and pushes the new environment and control structures onto the respective stacks.
    // It prepares the interpreter for interpreting the body of the lambda function.

    newEnv = createNewEnvironment();
    newEnv->assignParent(environmentMap.find(rator->associatedENV)->second);
    currEnvironment = newEnv;
    if (rator->variables.size() == 1)
    {
        currEnvironment->symbolTable[rator->variables.at(0)] = execStack.top();
        execStack.pop();
    }
    else
    {
        temp = execStack.top(); // Contain tuple pls
        execStack.pop();
        if (temp->type == Ctrl::TUPLE && rator->variables.size() == temp->ctrlTuples.size())
        {
            for (int i = 0; i < rator->variables.size(); i++)
            {
                currEnvironment->symbolTable[rator->variables.at(i)] = temp->ctrlTuples.at(i);
            }
        }
        else
        {
            cout << "Number/Type of arguments to a function doesn't match. rator variable size: " << rator->variables.size() << " temp tuples->size:" << temp->ctrlTuples.size() << endl;
            exit(1);
        }
    }
    environmentStack.push(currEnvironment);
    control.push_back(new Ctrl(Ctrl::ENV, currEnvironment->id, false));
    execStack.push(new Ctrl(Ctrl::ENV, currEnvironment->id, false));
    for (int i = 0; i < deltas->at(rator->index)->ctrlStruct->size(); i++)
    {
        control.push_back(deltas->at(rator->index)->ctrlStruct->at(i));
    }
}

void CSE::rule12(Ctrl *temp, Ctrl *currControl, Ctrl *rator, Env *newEnv, int deltaIndex)
{
    // Function to handle Rule 12 for interpreting Y* (Y-combinator) expressions.
    // The function creates a new environment and pushes it onto the environment stack to handle recursive function calls.

    Ctrl *eta = new Ctrl(execStack.top());
    execStack.pop();
    eta->type = Ctrl::ETA;
    execStack.push(eta);
}

void CSE::rule13(Ctrl *temp, Ctrl *currControl, Ctrl *rator, Env *newEnv, int deltaIndex)
{
    // Function to handle Rule 13 for interpreting Eta (eta-expansion) expressions.
    // The function prepares the interpreter to apply the operator in the eta-expansion.
    control.push_back(new Ctrl(Ctrl::GAMMA));
    control.push_back(new Ctrl(Ctrl::GAMMA));
    execStack.push(rator);
    Ctrl *lambda = new Ctrl(Ctrl::LAMBDA, &(rator->variables), NULL, rator->index);
    lambda->associatedENV = rator->associatedENV;
    execStack.push(lambda);
}

void CSE::rule10(Ctrl *temp, Ctrl *currControl, Ctrl *rator, Env *newEnv, int deltaIndex)
{
    // Function to handle Rule 10 for indexing tuples.
    // The function applies indexing on tuples and pushes the corresponding element onto the stack.

    if (execStack.top()->type == Ctrl::INTEGER)
    {
        temp = rator->ctrlTuples.at(atoi(execStack.top()->ctrlVal.c_str()) - 1);
        execStack.pop();
        execStack.push(temp);
    }
    else
    {
        cout << "Expected an integer while indexing tuples!";
        exit(1);
    }
}

void CSE::handleGAMMA(Ctrl *temp, Ctrl *currControl, Ctrl *rator, Env *newEnv, int deltaIndex)
{
    // Function to handle GAMMA (function application) during interpretation.
    // The function determines the type of the operator (rator) and applies the appropriate rule (e.g., Rule 4.11 for lambdas, Rule 1.2 for Y*, Rule 1.3 for Eta, etc.).
    // It also handles the application of built-in functions.

    control.pop_back();
    rator = execStack.top();
    execStack.pop();
    if (rator->type == Ctrl::LAMBDA)
    {
        rule411(temp, currControl, rator, newEnv, deltaIndex);
    }
    else if (rator->type == Ctrl::YSTAR)
    {
        rule12(temp, currControl, rator, newEnv, deltaIndex);
    }
    else if (rator->type == Ctrl::ETA)
    {
        rule13(temp, currControl, rator, newEnv, deltaIndex);
    }
    else if (rator->type == Ctrl::TUPLE)
    { // Rule 10
        rule10(temp, currControl, rator, newEnv, deltaIndex);
    }
    else
    {
        applyThisRator(rator);
    }
}

void CSE::handleBeta(Ctrl *temp, Ctrl *currControl, Ctrl *rator, Env *newEnv, int deltaIndex)
{
    // Function to handle BETA (conditional branching) during interpretation.
    // The function handles conditional branching based on the result of a previous expression evaluation.
    // It pushes the appropriate control structures based on the result (TRUE or FALSE) onto the control stack.

    control.pop_back();
    if (execStack.top()->type == Ctrl::TRUE)
    {
        control.pop_back();
        if (control.at(control.size() - 1)->type == Ctrl::DELTA)
        {
            deltaIndex = control.at(control.size() - 1)->index;
            control.pop_back();
        }
        else
        {
            cout << "Delta Expected " << control.at(control.size() - 1)->type << endl;
            exit(1);
        }
    }
    else if (execStack.top()->type == Ctrl::FALSE)
    {
        if (control.at(control.size() - 1)->type == Ctrl::DELTA)
        {
            deltaIndex = control.at(control.size() - 1)->index;
            control.pop_back();
            control.pop_back();
        }
        else
        {
            cout << "Delta Expected, found, " << control.at(control.size() - 1)->type << endl;
            exit(1);
        }
    }
    else
    {
        cout << " '->' operator Expected a bool value" << endl;
        exit(1);
    }
    execStack.pop();
    for (int i = 0; i < deltas->at(deltaIndex)->ctrlStruct->size(); i++)
    {
        control.push_back(deltas->at(deltaIndex)->ctrlStruct->at(i));
    }
}

void CSE::handleEnv(Ctrl *temp, Ctrl *currControl, Ctrl *rator, Env *newEnv, int deltaIndex)
{
    // Function to handle ENV (environment marker) during interpretation.
    // The function pops the top element from the stack and checks if it matches the expected environment marker.
    // If they match, it signifies the end of the current environment, and the interpreter moves to the previous environment.

    temp = execStack.top();
    execStack.pop();
    if (execStack.top()->type == Ctrl::ENV && execStack.top()->index == currControl->index)
    {
        control.pop_back();
        execStack.pop();
        execStack.push(temp);

        environmentStack.pop();
        currEnvironment = environmentStack.top();
    }
    else
    {
        cout << "Env markers do not match!" << endl;
        exit(1);
    }
}

void CSE::handleTau(Ctrl *temp, Ctrl *currControl, Ctrl *rator, Env *newEnv, int deltaIndex)
{
    // Function to handle TAU (tuple creation) during interpretation.
    // The function creates a new tuple and pushes its
    control.pop_back();
    temp = new Ctrl();
    temp->type = Ctrl::TUPLE;
    temp->ctrlTuples.clear();
    for (int i = 0; i < currControl->index; i++)
    {
        temp->ctrlTuples.push_back(execStack.top());
        execStack.pop();
    }
    execStack.push(temp);
}

void CSE::handleNeg(Ctrl *temp, Ctrl *currControl, Ctrl *rator, Env *newEnv, int deltaIndex)
{
    // Function to handle Neg during interpretation.
    // The function creates a negation of its

    control.pop_back();
    if (execStack.top()->type == Ctrl::INTEGER)
        execStack.top()->ctrlVal = to_string(-1 * atoi(execStack.top()->ctrlVal.c_str()));
    else
    {
        cout << "Neg: Int not found" << endl;
        exit(1);
    }
}

// Executes the Control Structure Evaluation algorithm.
void CSE::execCSE()
{

    Ctrl *temp = NULL;
    Ctrl *currControl = NULL;
    Ctrl *rator = NULL;
    Env *newEnv = NULL;
    int deltaIndex = -1;
    while (!control.empty())
    {

        currControl = control.at(control.size() - 1);
        temp = NULL;
        switch (currControl->type)
        {
        // CSE Rule 1: Pushes simple control elements (INTEGER, STRING, TRUE, FALSE, DUMMY, YSTAR) to the execution stack.
        case Ctrl::INTEGER:
        case Ctrl::STRING:
        case Ctrl::TRUE:
        case Ctrl::FALSE:
        case Ctrl::DUMMY:
        case Ctrl::YSTAR:
            control.pop_back();
            execStack.push(currControl);
            break;
        case Ctrl::NAME:
            handleName(temp, currControl, rator, newEnv, deltaIndex);
            break;
        case Ctrl::NIL:
            currControl->ctrlTuples.clear();
            control.pop_back();
            execStack.push(currControl);
            break;
        case Ctrl::LAMBDA: // CSE Rule 2
            currControl->associatedENV = currEnvironment->id;
            control.pop_back();
            execStack.push(currControl);
            break;
        case Ctrl::GAMMA:
            handleGAMMA(temp, currControl, rator, newEnv, deltaIndex);
            break;
        case Ctrl::NOT:
            control.pop_back();
            temp = execStack.top();
            if (temp->type == Ctrl::TRUE || temp->type == Ctrl::FALSE)
            {
                temp->type = temp->type == Ctrl::TRUE ? Ctrl::FALSE : Ctrl::TRUE;
            }
            else
            {
                cout << "Expecting a bool value for operator 'not'" << endl;
                exit(1);
            }
            break;
        // CSE Rule 6
        case Ctrl::GR:
        case Ctrl::GE:
        case Ctrl::LS:
        case Ctrl::LE:
        case Ctrl::EQ:
        case Ctrl::NE:
        case Ctrl::AUG:
        case Ctrl::OR:
        case Ctrl::AND_LOGICAL:
        case Ctrl::ADD:
        case Ctrl::SUBTRACT:
        case Ctrl::MULTIPLY:
        case Ctrl::DIVIDE:
        case Ctrl::EXP:
            applyBinaryOPR(currControl->type);
            break;
        case Ctrl::TAU:
            handleTau(temp, currControl, rator, newEnv, deltaIndex);
            break;
        // CSE Rule 7
        case Ctrl::NEG:
            handleNeg(temp, currControl, rator, newEnv, deltaIndex);
            break;
        case Ctrl::ENV:
            handleEnv(temp, currControl, rator, newEnv, deltaIndex);
            break;
        case Ctrl::BETA:
            handleBeta(temp, currControl, rator, newEnv, deltaIndex);
            break;
        default:
            cout << "Unknown Ctrl Type: " << currControl->type << endl;
            break;
        }
    }
}

Env *CSE::createNewEnvironment()
{

    tempEnvironment = new Env(numEnvironment);
    environmentMap[numEnvironment] = tempEnvironment;
    numEnvironment++;
    return tempEnvironment;
}

void CSE::printCS()
{

    for (int i = 0; i < control.size(); i++)
        cout << control.at(i)->toStr() << " ";

    printf("---------");
    stack<Ctrl *> temp;

    while (!execStack.empty())
    {

        cout << execStack.top()->toStr() << " ";
        temp.push(execStack.top());
        execStack.pop();
    }

    printf("\n");
    while (!temp.empty())
    {
        execStack.push(temp.top());
        temp.pop();
    }
}

void CSE::init(Node *root)
{

    deltas->push_back(rootDelta);
    flattenTree(root, rootDelta, deltas);
    control.push_back(new Ctrl(Ctrl::ENV, 0, false));
    execStack.push(new Ctrl(Ctrl::ENV, 0, false));

    for (int i = 0; i < rootDelta->ctrlStruct->size(); i++)
        control.push_back(rootDelta->ctrlStruct->at(i));

    environmentStack.push(createNewEnvironment());
    environmentStack.top()->assignParent(PE);
    currEnvironment = environmentStack.top();
}

void CSE::run(Node *root)
{

    init(root);
    execCSE();

    if (!control.empty() || environmentStack.size() != 1)
    {
        printf("stack/env_stack is not empty");
    }

    printf("\n");
}

void CSE::flattenDeltaThen(Node *node, Ctrl *delta, vector<Ctrl *> *deltas)
{

    Ctrl *deltaThen = new Ctrl(Ctrl::DELTA, deltas->size());
    deltas->push_back(deltaThen);
    delta->ctrlStruct->push_back(new Ctrl(Ctrl::DELTA, deltas->size() - 1)); // delta then
    if (node->childNode->siblingNode->type == Node::TERNARY)
    {
        flattenTree(node->childNode->siblingNode, deltaThen, deltas);
    }
    else
    {

        vector<string> *tempvariables = NULL;
        if (node->childNode->siblingNode->type == Node::TAU)
        {
            Node *temp = node->childNode->siblingNode->childNode;
            tempvariables = new vector<string>;
            while (temp != NULL)
            {
                tempvariables->push_back(temp->nodeString); // will these be any useful
                temp = temp->siblingNode;
            }
        }
        deltaThen->addCtrl(node->childNode->siblingNode, node->childNode->siblingNode->type, node->childNode->siblingNode->nodeString, tempvariables, deltaThen, deltas->size());
        if (node->childNode->siblingNode->childNode != NULL)
            flattenTree(node->childNode->siblingNode->childNode, deltaThen, deltas);
    }
}

void CSE::flattenDeltaElse(Node *node, Ctrl *delta, vector<Ctrl *> *deltas)
{

    Ctrl *deltaElse = new Ctrl(Ctrl::DELTA, deltas->size());
    deltas->push_back(deltaElse);
    delta->ctrlStruct->push_back(new Ctrl(Ctrl::DELTA, deltas->size() - 1));

    if (node->childNode->siblingNode->siblingNode->type == Node::TERNARY)
    {
        flattenTree(node->childNode->siblingNode->siblingNode, deltaElse, deltas);
    }
    else
    {
        vector<string> *tempvariables = NULL;
        if (node->childNode->siblingNode->siblingNode->type == Node::TAU)
        {

            Node *temp = node->childNode->siblingNode->siblingNode->childNode;
            tempvariables = new vector<string>;

            while (temp != NULL)
            {
                tempvariables->push_back(temp->nodeString);
                temp = temp->siblingNode;
            }
        }
        deltaElse->addCtrl(node->childNode->siblingNode->siblingNode, node->childNode->siblingNode->siblingNode->type, node->childNode->siblingNode->siblingNode->nodeString, tempvariables, deltaElse, deltas->size());
        if (node->childNode->siblingNode->siblingNode->childNode != NULL)
            flattenTree(node->childNode->siblingNode->siblingNode->childNode, deltaElse, deltas);
    }
}

void CSE::flattenTernary(Node *node, Ctrl *delta, vector<Ctrl *> *deltas)
{

    flattenDeltaThen(node, delta, deltas);

    flattenDeltaElse(node, delta, deltas);

    Ctrl *beta = new Ctrl(Ctrl::BETA);
    delta->ctrlStruct->push_back(new Ctrl(Ctrl::BETA, "beta"));
    delta->addCtrl(node->childNode, node->childNode->type, node->childNode->nodeString, NULL, NULL, deltas->size());
    if (node->childNode->childNode != NULL)
        flattenTree(node->childNode->childNode, delta, deltas);
}

void CSE::flattenLAMBDA(Node *node, Ctrl *delta, vector<Ctrl *> *deltas)
{

    Ctrl *temp = NULL;
    vector<string> *variables = NULL;
    variables = new vector<string>();
    if (Node::IDENTIFIER == node->childNode->type)
    {
        variables->push_back(node->childNode->nodeString);
    }
    else if (Node::COMMA == node->childNode->type)
    {
        Node *temp = node->childNode->childNode;
        while (NULL != temp)
        {
            variables->push_back(temp->nodeString);
            temp = temp->siblingNode;
        }
    }
    else
    {
        cout << "Expected Identifier or Comma, but din't find" << endl;
    }
    temp = new Ctrl(Ctrl::DELTA, deltas->size());
    deltas->push_back(temp);
    delta->addCtrl(node, node->type, node->nodeString, variables, temp, deltas->size());
    flattenTree(node->childNode->siblingNode, temp, deltas);

    if (NULL != node->siblingNode)
        flattenTree(node->siblingNode, delta, deltas);
}

void CSE::flattenTree(Node *node, Ctrl *delta, vector<Ctrl *> *deltas)
{

    Ctrl *temp_del_ptr = NULL;
    vector<string> *variables = NULL;
    if (Node::LAMBDA == node->type)
    {
        flattenLAMBDA(node, delta, deltas);
    }
    else if (node->type == Node::TERNARY)
    {
        flattenTernary(node, delta, deltas);
    }
    else
    {
        if (node->type == Node::TAU)
        {
            variables = new vector<string>();
            Node *temp = node->childNode;
            while (temp != NULL)
            {
                variables->push_back(temp->nodeString);
                temp = temp->siblingNode;
            }
        }
        delta->addCtrl(node, node->type, node->nodeString, variables, temp_del_ptr, deltas->size());
        if (NULL != node->childNode)
        {
            flattenTree(node->childNode, delta, deltas);
        }
        if (NULL != node->siblingNode)
        {
            flattenTree(node->siblingNode, delta, deltas);
        }
    }
}
