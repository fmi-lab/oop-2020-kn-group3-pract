# Dummy title
Това, което (би трябвало да) сте разбрали, упражнили и да сте се научили да правите, накратко:

1. 21.II. **Структури и функции върху тях:**
    - структурите - множество променливи под общо име, представляващи някаква смислова цялост
    - инициализация с къдрави скоби
    - методи - функции, закачени към структурите (spoilers: classes)
        - указателят this
        - директен достъп до член-данните на структурата, подобно на локални променливи
    - [DRY](https://en.wikipedia.org/wiki/Don%27t_repeat_yourself)!

2. 28.II. **Конструктори и деструктори**
    - Въпросът _Защо?_. Мотивация за нуждата от конструктори и деструктори.
    - Проблемът с динамичната памет - заделяне и освобождаване
    - Синтаксис на кон- и деструкторите. Как се ползват и какво се случва отзад.

3. 06.III. **Динамично преоразмеряващи се масиви. Шаблони.**
    - Нуждата от динамично преоразмеряващи се масиви
    - По-сложна функционалност - push & erase
    - _Внимателна_ работа с динамичната памет, пренасочване на указателите
    - И още: реализирайте следните функции като член-функции или свободни функции по ваша преценка:
        - `extendWith` - добавя елементите на един DynamicArray към друг.
        - `deleteRange` - по дадени начален индекс start и дължина length, изтрива елементите с индекси [start, start + length).
        - Шаблонна член-функция `template<typename U> DynamicArray<U> map(U (*f)(T const&))`, която по даден прекарва елементите на динамичния масив през подадената функция и създава нов динамичен масив от тип `DynamicArray<U>` (върху типа `U`). Защо това е полезно?

4. 20.III. **Видимост на член-данните и методите. Класове** & още динамична памет
    - `public` & `private` - какво, защо, къде?
        - Произтичащата оттук нужда от get() и set()-like методи (getters, setters)
        - Защо ползваме `private:` с get/set методи за достъп, вместо директно самите данни с `public:`?
    - Задачата за [матрицата](./04.sources/README.md)
    - Проблемът с double delete [] - откъде идва, как се решава? (Copy c-tor, operator=())
    - Ръчно дефиниране на `operator[](int n)`, за да правим `myArray[5]` върху собствени обекти.

5. 31.III. **Headers & source files, forward declarations**
    - [Код](./not-named-yet)
	- `union`
