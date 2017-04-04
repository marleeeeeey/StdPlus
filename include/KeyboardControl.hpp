#ifndef KeyboardControl_h__
#define KeyboardControl_h__

// How to use
// key - valueUP, shift+key - valueDown, h - help (print all keys and values)
//													name       key   step   init
// double minAreaRatio = Keyboard<double>::get("minAreaRatio", 'a', 0.0001, 0.01);

#include "MacrosPlus.hpp"

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
                << name() << " = " << stdplus::to_string(value())
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

    class KeyboardControl
    {
    public:
        static KeyboardControl & instanse()
        {
            static KeyboardControl keyboardControl;
            return keyboardControl;
        }

        template<typename T>
        static IKeyboardAction * addAction(const std::string & name, char keyChar, T & refData, T step)
        {
            KeyboardControl & kc = KeyboardControl::instanse();
            IKeyboardAction * act = new KeyboardAction<T>(name, keyChar, refData, step);
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
            IKeyboardAction * act = new KeyboardAction<double>("__help__", 'h', tempValue, 0);
            addAction(act);
        }

        IKeyboardAction * addAction(IKeyboardAction * pNewAction, bool isExcept = true)
        {
            if (tolower(pNewAction->keyChar()) == m_keyHelp)
                stdplus::throwExcept("Never used key 'H' for your actions");

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
                char ch = getch();

                if (ch == m_keyExit)
                    exit(0);

                if (tolower(ch) == tolower(m_keyHelp))
                {
                    printAll();
                    continue;
                }

                if (m_isStop) break;

                {
                    std::lock_guard<std::mutex> guard(m_mutexActions);

                    for (IKeyboardAction * keyAction : m_actions)
                    {
                        char lower = tolower(keyAction->keyChar());
                        char upper = toupper(keyAction->keyChar());

                        if (ch == upper)
                            keyAction->valueDown();

                        if (ch == lower)
                            keyAction->valueUp();
                    }
                }
            }

            AMSG("Stop " + AFUNSIG);
        }

        inline void stop() { m_isStop = true; }

        void printAll()
        {
            for (IKeyboardAction * act : m_actions)
            {
                AMSG(act->toString());
            }
        }

        std::mutex                     m_mutexActions;
        std::vector<IKeyboardAction *> m_actions;
        bool                           m_isStop = false;
        char                           m_keyHelp = 'h';
        char                           m_keyExit = 27;
    };

    template<typename T>
    class Keyboard
    {
    public:
        static T get(const std::string & name, char keyChar, T step = 1, T initValue = default(T))
        {
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

#endif // KeyboardControl_h__

