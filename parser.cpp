#include "parser.h"

#include <QDebug>



Parser::Parser(QObject *parent) : QObject(parent), data("")
{

}

Parser::Parser(QString data) :
    data(data)
{
    fillingInTheListOfTokens(data);
    fillingInTheListOfCalculations();
    calculate();
    int i;
}

void Parser::fillingInTheListOfTokens(QString data)
{
    QString rest = data.replace("\n"," ");
    while(  rest != "")
    {
        QString  current = rest.left(rest.indexOf(";"));
        if(current != "")
            parsesAStringInTokens(current);
        rest = rest.mid(rest.indexOf(";")+ 1,rest.size());
    }


}


void Parser::parsesAStringInTokens(QString str)
{
    str = removingSpacesInTheBegin(str) + " ";
    QString tokin = "";
    for (int i(0); i < str.size(); i++)
    {
        QString currentChar = str.at(i);
        if(currentChar != " ")
        {
            tokin += currentChar;
        }
        else
        {
            if( tokin != "" )
                tokinList.push_back(tokin);
            tokin = "";
        }


    }
    tokinList.push_back(";");


}

QString Parser::removingSpacesInTheBegin(QString str)
{
    int index = 0;
    while(" " == str.at(index))
        index++;
    return str.mid(index,str.size());

}




void Parser::fillingInTheListOfCalculations()
{
    QVector<QString> stack;
    while(!tokinList.empty())
    {
        if(tokinList.front() == ";")
        {
            while( (!stack.empty()) && ("{" != stack.back()) )
            {
                listOfCalculations.push_back(stack.back());
                stack.pop_back();
            }
            tokinList.pop_front();
        }
        else if(!isOperation(tokinList.front()))
        {
            listOfCalculations.push_back(tokinList.front());
            tokinList.pop_front();
        }
        else if((tokinList.front() == ")" || tokinList.front() == "("))
        {
            if(tokinList.front() == "(" )
            {
                stack.push_back(tokinList.front());
                tokinList.pop_front();
            }
            else
            {
                tokinList.pop_front();
                while("(" != stack.back() )
                {
                    listOfCalculations.push_back(stack.back());
                    stack.pop_back();
                }
                stack.pop_back();
            }
        }
        else if( (tokinList.front() == "{" || tokinList.front() == "}") )
        {
            if(tokinList.front() == "{")
            {
                listOfCalculations.push_back(stack.back());
                stack.pop_back();
                stack.push_back(tokinList.front());
                listOfCalculations.push_back(tokinList.front());
                tokinList.pop_front();
            }
            else
            {
                while("{" != stack.back() )
                {
                    listOfCalculations.push_back(stack.back());
                    stack.pop_back();
                }
                stack.pop_back();
                listOfCalculations.push_back(tokinList.front());
                tokinList.pop_front();
            }
        }
        else
        {
            if(!stack.empty())
            {
                if( priorityOfOperations(tokinList.front()) > priorityOfOperations( stack.back() ) )
                {
                    stack.push_back(tokinList.front());
                    tokinList.pop_front();
                }
                else
                {
                    while(priorityOfOperations(tokinList.front()) <= priorityOfOperations( stack.back() ) )
                    {
                        listOfCalculations.push_back(stack.back());
                        stack.pop_back();
                    }
                    stack.push_back(tokinList.front());
                    tokinList.pop_front();
                }
            }
            else
            {
                stack.push_back(tokinList.front());
                tokinList.pop_front();

            }
        }

    }
}



bool Parser::isOperation(QString tokin)
{
    if(tokin == "print")
        return true;
    else if( tokin == "if" || tokin == "{" || tokin == "}" || tokin == "else")
        return true;
    else if( tokin == "(" ||  tokin == ")"  || tokin == "true" || tokin == "flase" )
        return true;
    else if( tokin == "=")
        return true;
    else if(tokin == "<")
        return true;
    else if( tokin == "+")
        return true;
    else if( tokin == "-")
        return true;
    else if(  tokin == "*")
        return true;
    else if( tokin == "/" )
        return true;
    else
        return false;


}

int Parser::priorityOfOperations(QString tokin)
{
    if(tokin == "print")
        return 0;
    else if( tokin == "if")
        return 0;
    else if( tokin == "{" || tokin == "}")
        return -1;
    else if( tokin == "(" ||  tokin == ")"  || tokin == "true" || tokin == "flase")
        return -2;
    else if( tokin == "=")
        return 1;
    else if(tokin == "<")
        return 2;
    else if( tokin == "+")
        return 3;
    else if( tokin == "-")
        return 3;
    else if(  tokin == "*")
        return 4;
    else if( tokin == "/" )
        return 4;
    else if( tokin == "else")
        return 5;
    else
        return 0;
}


void Parser::calculate()
{

    listOfCalculations.push_back("");
    while(!listOfCalculations.empty())
    {
        QString tokin = listOfCalculations.front();
        if(  isOperation(tokin)   )
        {
            performingAnOperation( tokin);
            listOfCalculations.pop_front();


        }
        else
        {
            stack.push_back(tokin);
            listOfCalculations.pop_front();
        }
    }

}


void Parser::performingAnOperation(QString operation)
{
    if(operation == "print")
    {
        listOfCalculations.pop_front();
        rezult += QString::number( valueOfVariable( stack.back() ) );
    }
    else if(operation == "if")
    {

    }
    else if( operation == "(" ||  operation == ")" )
        return ;
    else if( operation == "=")
    {
        QString rightOperands = stack.back();
        stack.pop_back();
        QString leftOperands = stack.back();
        stack.pop_back();

        bool flag = true;
        listVar[leftOperands] = rightOperands.toDouble(&flag);

        // if(!flag)
        //throw;
    }
    else if(operation == "<")
    {
        double rightOperands = valueOfVariable( stack.back() );
        stack.pop_back();


        double leftOperands = valueOfVariable( stack.back() );
        stack.pop_back();

        if( leftOperands < rightOperands)
        {
            stack.push_back("true");
        }
        else
        {
            stack.push_back("flase");
        }
    }
    else if( operation == "+")
    {
        double rightOperands = valueOfVariable( stack.back() );
        stack.pop_back();


        double leftOperands = valueOfVariable( stack.back() );
        stack.pop_back();


        stack.push_back(QString::number(  leftOperands + rightOperands  ));

    }
    else if( operation == "-")
    {
        double rightOperands = valueOfVariable( stack.back() );
        stack.pop_back();


        double leftOperands = valueOfVariable( stack.back() );
        stack.pop_back();


        stack.push_back(QString::number(  leftOperands - rightOperands  ));

    }
    else if(  operation == "*")
    {
        double rightOperands = valueOfVariable( stack.back() );
        stack.pop_back();


        double leftOperands = valueOfVariable( stack.back() );
        stack.pop_back();


        stack.push_back(QString::number(  leftOperands * rightOperands  ));

    }
    else if( operation == "/" )
    {
        double rightOperands = valueOfVariable( stack.back() );
        stack.pop_back();


        double leftOperands = valueOfVariable( stack.back() );
        stack.pop_back();


        stack.push_back(QString::number(  leftOperands / rightOperands  ));

    }
    else
        return;
}

double Parser::valueOfVariable(QString var)
{
    bool flag = true;
    double value = var.toDouble(&flag);
    if(!flag)
    {
        value = listVar[var];
    }
    return value;
}

QString Parser::getRezult()
{
    return rezult;
}


