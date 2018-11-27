/*!
 * \author Tyulenev Sergey
 * Contact: marleeeeeey@gmail.com
 * last version https://github.com/marleeeeeey/StdPlus
*/

#pragma once

// How to use (this is old style)
// key - valueUP, shift+key - valueDown, h - help (print all keys and values)
//													name       key   step   init
// double minAreaRatio = Keyboard<double>::get("minAreaRatio", 'a', 0.0001, 0.01);

#include "MacrosPlus.hpp"


#define ABIND_INT(var, initValue)              (var) = stdplus::Keyboard<int>::   get(#var, (initValue), 0,    1)
#define ABIND_INT_STEP(var, initValue, step)   (var) = stdplus::Keyboard<int>::   get(#var, (initValue), 0, step)
#define ABIND_DBL(var, initValue)              (var) = stdplus::Keyboard<double>::get(#var, (initValue), 0,    1)
#define ABIND_DBL_STEP(var, initValue, step)   (var) = stdplus::Keyboard<double>::get(#var, (initValue), 0, step)


#define LOG_STD_BIND_INT(var, initValue)               ABIND_INT(var, initValue)           
#define LOG_STD_BIND_INT_STEP(var, initValue, step)    ABIND_INT_STEP(var, initValue, step)
#define LOG_STD_BIND_DBL(var, initValue)               ABIND_DBL(var, initValue)           
#define LOG_STD_BIND_DBL_STEP(var, initValue, step)    ABIND_DBL_STEP(var, initValue, step)


namespace stdplus
{
    class IKeyboardAction
    {
        friend class KeyboardControl;

    public:
        IKeyboardAction(const std::string name, char keyChar)
        {
            m_name = name;
            m_keyChar = keyChar;
        }

        inline std::string name() const { return m_name; }
        inline char keyChar() const { return m_keyChar; }      

        virtual std::string toString() = 0;

    private:
        virtual void valueUp() = 0;
        virtual void valueDown() = 0;

        std::string m_name;
        char		m_keyChar;

    };

    template<typename T>
    class KeyboardAction : public IKeyboardAction
    {
    public:
        KeyboardAction(const std::string & name, char keyChar, T & refData, T step)
            : IKeyboardAction(name, keyChar)
            , m_refData(refData)
            , m_step(step)
        { }

        T value() const { return m_refData; }

        virtual std::string toString() override
        {
            std::ostringstream ss;

			ss
				<< "[" << keyChar() << "] "
				<< std::setw(20) << std::setiosflags(std::ios_base::left) 
				<< name() 
				<< " = " 
				<< std::setw(5) << std::setiosflags(std::ios_base::right) 
				<< stdplus::to_string(value())
                ;

            return ss.str();
        }

    private:

        virtual void valueUp() override
        {
            m_refData += m_step;
            AMSG(this->toString());
        }

        virtual void valueDown() override
        {
            m_refData -= m_step;
            AMSG(this->toString());
        }


        T  & m_refData;
        T	 m_step;
            
    };

    // *************************** Storage and work with KeyboardActions ***************************
    
    class KeyboardControl
    {
    public:

        inline static KeyboardControl & instanse()
        {
            static KeyboardControl keyboardControl;
            return keyboardControl;
        }

        template<typename T>
        static IKeyboardAction * addAction(const std::string & name, char keyChar, T & refData, T step)
        {
            KeyboardControl & kc = KeyboardControl::instanse();
            IKeyboardAction * act = new KeyboardAction<T>(name, keyChar, refData, step);
            AMSG("add new action " + act->toString());
            return kc.addAction(act, false);
        }

        IKeyboardAction * isExistAction(const std::string & name, char keyChar)
        {
            for (IKeyboardAction * pOldAction : m_actions)
            {
                if (pOldAction->keyChar() == keyChar
                    || pOldAction->name() == name)
                {
                    return pOldAction;
                }
            }

            return nullptr;
        }

        inline IKeyboardAction * isExistKeyAction(char keyChar)
        {
            for (IKeyboardAction * pOldAction : m_actions)
            {
                if (pOldAction->keyChar() == keyChar)
                    return pOldAction;
            }

            return nullptr;
        }

    private:

        KeyboardControl()
        {
            std::thread(&KeyboardControl::process, this).detach();
        }

        KeyboardControl(const KeyboardControl &) = delete;
        void operator=(const KeyboardControl &) = delete;

        void addHelpAction()
        {
            double * pTempValue = new double;
            double & tempValue = *pTempValue;
            IKeyboardAction * act = new KeyboardAction<double>("__help__", m_keyHelp, tempValue, 0);
            addAction(act);
        }

        IKeyboardAction * addAction(IKeyboardAction * pNewAction, bool isExcept = true)
        {
            if (tolower(pNewAction->keyChar()) == m_keyHelp)
                stdplus::throwExcept("Never used key '" + to_string(m_keyHelp) + "' for your actions");

            std::lock_guard<std::mutex> guard(m_mutexActions);

            IKeyboardAction * pExistAction = isExistAction(pNewAction->name(), pNewAction->keyChar());

            if (pExistAction != nullptr)
            {
                std::string errorMsg = "Already Exist Action. Action not push_back !!!";

                if (isExcept)
                {
                    stdplus::throwExcept(errorMsg);
                }
                else
                {
                    AMSG(errorMsg);
                    return pExistAction;
                }
            }

            m_actions.push_back(pNewAction);

            return pNewAction;
        }

        void process()
        {
            AFUN;

            while (true)
            {
                char ch = std::cin.get();//_getch();

                if (ch == m_keyExit)
                    exit(0);

                if (tolower(ch) == tolower(m_keyHelp))
                {
                    printAll();
                    continue;
                }

                if (m_isStop) break;
                
                std::lock_guard<std::mutex> guard(m_mutexActions);

                for (IKeyboardAction * keyAction : m_actions)
                {
                    char lower = tolower(keyAction->keyChar());
                    char upper = toupper(keyAction->keyChar());

                    if (ch == upper)
                        keyAction->valueDown();
                    else if (ch == lower)
                        keyAction->valueUp();
                }

            }

            AMSG("Stop " + AFUNSIG);
        }

        inline void stop() { m_isStop = true; }

        void printAll()
        {
			ASPLIT;
            for (IKeyboardAction * act : m_actions)
            {
                AMSG(act->toString());
            }
            ASPLIT;
        }

        std::mutex                     m_mutexActions;
        std::vector<IKeyboardAction *> m_actions;
        bool                           m_isStop = false;
        char                           m_keyHelp = 'z';
        char                           m_keyExit = 27;
    };

    // **************************** storage values data in heap ****************************

    // storage values data in heap
    template<typename T>
    class Keyboard
    {
    public:
        static T get(const std::string & name, T initValue = 0, char keyChar = '\0', T step = 1)
        {
            if (keyChar == 0)
            {
                const char startKey = 'a';
                const char endKey   = 'z';

                for (char ch = startKey; ch <= endKey; ch++)
                {
                    if (!KeyboardControl::instanse().isExistKeyAction(ch))
                    {
                        keyChar = ch;
                        break;
                    }
                }

                AEXCEPT_IF(keyChar == 0);
            }

            IKeyboardAction * pAction = KeyboardControl::instanse().isExistAction(name, keyChar);

            if (pAction == nullptr)
            {
                T * value = new T(initValue);
                pAction = KeyboardControl::addAction(name, keyChar, *value, step);
            }

            KeyboardAction<T> * realAcion = dynamic_cast<KeyboardAction<T> *>(pAction);

            return realAcion->value();
        }

        static std::vector<T *> m_values;
    };

    template<typename T>
    std::vector<T *> Keyboard<T>::m_values;

}

