#include "Ctrl.h"

// Function to convert the Ctrl type to a string representation
string Ctrl::toStr()
{

    string str;

    // Handle different cases for different Ctrl types
    switch (type)
    {

    case DELTA:
        str = "<D" + std::to_string(index) + ">";
        return str;

    case NAME:
        return variables.at(0); // Return the first element of the 'variables' vector

    case LAMBDA:
        str = "[lambda closure: ";

        for (int i = 0; i < variables.size(); i++)
            str += variables.at(i) + ": ";

        str += to_string(index) + "]";
        return str;

    // For other cases, directly return the corresponding strings or values
    case GAMMA:
        return "Gamma";

    case AUG:
        return "AUG";

    case BETA:
        return "BETA";

    case OR:
        return "OR";

    case AND_LOGICAL:
        return "AND";

    case NOT:
        return "NOT";

    case GR:
        return ">";

    case GE:
        return ">=";

    case LS:
        return "<";

    case LE:
        return "<=";

    case EQ:
        return "=";

    case NE:
        return "!=";

    case ADD:
        return "+";

    case SUBTRACT:
        return "-";

    case NEG:
        return "NEG";

    case MULTIPLY:
        return "*";

    case DIVIDE:
        return "/";

    case EXP:
        return "**";

    case AT:
        return "@";

    case TRUE:
        return "true";

    case FALSE:
        return "false";

    case NIL:
        return "nil";

    case DUMMY:
        return "dummy";

    case YSTAR:
        return "Y";

    case TAU:
        str = "<T" + to_string(index) + ">";
        return str;

    case STRING:
        return ctrlVal + "";

    case INTEGER:
        str = ctrlVal + "";
        return str;

    case ENV:
        str = "e" + to_string(index);
        return str;

    case ETA:
        str = "<ETA," + to_string(index);

        for (int i = 0; i < variables.size(); i++)
            str += "," + variables.at(i);

        str += ">";
        return str;

    case TUPLE:
        str = "(";

        for (int i = 0; i < ctrlTuples.size(); i++)
        {

            str += ctrlTuples.at(i)->toStr();
            if (i != ctrlTuples.size() - 1)
                str += ", ";
        }

        str += ")";
        return str;

    default:
        return "unknown";
    }
}
// Constructor to create a Ctrl object from an existing Ctrl object
Ctrl::Ctrl(Ctrl *ctrl)
{
    // Copy the values from the 'ctrl' object to the new object being constructed
    index = ctrl->index;
    ctrlVal = ctrl->ctrlVal;
    type = ctrl->type;
    associatedENV = ctrl->associatedENV;

    if (!ctrl->variables.empty())
    {
        for (int i = 0; i < ctrl->variables.size(); i++)
            variables.push_back(ctrl->variables.at(i));
    }

    if (!ctrl->ctrlTuples.empty())
    {
        for (int i = 0; i < ctrl->ctrlTuples.size(); i++)
            ctrlTuples.push_back(ctrl->ctrlTuples.at(i));
    }
}

Ctrl::Ctrl(Ctrl::Type type, int index)
{

    if (Ctrl::DELTA != type)
    {
        printf("Ctrl::DELTA 's constructor called for : %d", type);
    }

    this->type = type;
    this->index = index;
    ctrlStruct = new vector<Ctrl *>();
}

Ctrl::Ctrl(Ctrl::Type type, vector<string> *vars, Ctrl *del_ptr, int deltaindex)
{

    this->type = type; // Must be LAMBDA
    index = deltaindex;

    if (vars != NULL)
    {

        for (int i = 0; i < vars->size(); i++)
        {
            variables.push_back(vars->at(i));
        }
    }
}

Ctrl::Ctrl(Ctrl::Type type, int index, bool b)
{

    if (type != Ctrl::TAU && type != Ctrl::ENV)
        printf("Ctrl::TAU 's constructor called for : %d", type);

    this->type = type;
    this->index = index;
}

Ctrl::Ctrl(string var_value, Ctrl::Type type)
{

    this->type = type;
    variables.push_back(var_value);
}

Ctrl::Ctrl(Ctrl::Type type, string value)
{

    this->type = type;
    ctrlVal = value;
}

Ctrl::Ctrl()
{
}

Ctrl::Ctrl(Ctrl::Type type)
{

    this->type = type;
}

// Function to add a Ctrl object to the 'ctrlStruct' vector based on the given Node type
void Ctrl::addCtrl(Node *node, int type, string value, vector<string> *variables, Ctrl *del_ptr, int deltas_size)
{

    int tau_count;
    Ctrl *temp = NULL;

    switch (node->type)
    {
    // Handle different cases for different Node types and create corresponding Ctrl objects
    case Node::LAMBDA:
        temp = new Ctrl(Ctrl::LAMBDA, variables, del_ptr, deltas_size - 1);
        break;

    case Node::INTEGER:
        // Remove <ID: and > from the node string before storing it
        temp = new Ctrl(Ctrl::INTEGER, node->nodeString);
        break;

    case Node::MULTIPLY:
        temp = new Ctrl(Ctrl::MULTIPLY, node->nodeString);
        break;

    case Node::ADD:
        temp = new Ctrl(Ctrl::ADD, node->nodeString);
        break;

    case Node::SUBTRACT:
        temp = new Ctrl(Ctrl::SUBTRACT, node->nodeString);
        break;

    case Node::DIVIDE:
        temp = new Ctrl(Ctrl::DIVIDE, node->nodeString);
        break;

    case Node::GAMMA:
        temp = new Ctrl(Ctrl::GAMMA, node->nodeString);
        break;

    case Node::IDENTIFIER:
        temp = new Ctrl(node->nodeString, Ctrl::NAME);
        break;

    case Node::STRING:
        // Remove <ID: and > from the node string before storing it
        temp = new Ctrl(Ctrl::STRING, node->nodeString.substr(1, node->nodeString.length() - 2));
        break;

    case Node::TAU:
        if (variables != NULL)
            tau_count = variables->size();
        else
            cout << "TAU add_control NULL variables sent!" << endl;
        temp = new Ctrl(Ctrl::TAU, tau_count, false);
        break;

    case Node::AUG:
        temp = new Ctrl(Ctrl::AUG);
        break;

    case Node::NIL:
        temp = new Ctrl(Ctrl::NIL);
        break;

    case Node::YSTAR:
        temp = new Ctrl(Ctrl::YSTAR);
        break;

    case Node::AND_LOGICAL:
        temp = new Ctrl(Ctrl::AND_LOGICAL);
        break;

    case Node::OR:
        temp = new Ctrl(Ctrl::OR);
        break;

    case Node::NE:
        temp = new Ctrl(Ctrl::NE);
        break;

    case Node::EQ:
        temp = new Ctrl(Ctrl::EQ);
        break;

    case Node::LS:
        temp = new Ctrl(Ctrl::LS);
        break;

    case Node::LE:
        temp = new Ctrl(Ctrl::LE);
        break;

    case Node::GR:
        temp = new Ctrl(Ctrl::GR);
        break;

    case Node::GE:
        temp = new Ctrl(Ctrl::GE);
        break;

    case Node::NEG:
        temp = new Ctrl(Ctrl::NEG);
        break;

    case Node::FALSE:
        temp = new Ctrl(Ctrl::FALSE);
        break;

    case Node::TRUE:
        temp = new Ctrl(Ctrl::TRUE);
        break;

    case Node::NOT:
        temp = new Ctrl(Ctrl::NOT);
        break;

    case Node::DUMMY:
        temp = new Ctrl(Ctrl::DUMMY);
        break;

    default:
        printf("This treenode node handled. val: %s type: %d", value.c_str(), type);
        break;
    };
    // Add the newly created Ctrl object 'temp' to the 'ctrlStruct' vector
    ctrlStruct->push_back(temp);
}
