# Подготовительная программа на С/С++
## Подготовка
Необходимо форкнуть репу, сделать её приватной и добавить пользователей:
1. mrGexogen
2. pberejnoy
3. Antonboom
4. grechkin-pogrebnyakov

с правами **Developer**.

[fork]: images/fork.png "Fork"
[permissions]: images/permissions.png "Permissions"
[members]: images/members.png "Members"
[merge_request_1]: images/merge_request_1.png "Merge Request 1"
[merge_request_2]: images/merge_request_2.png "Merge Request 2"

### Fork:
![alt text][fork]
### Permissions:
![alt text][permissions]
### Members:
![alt text][members]

### Локальные настройки GIT'а

Просим вас исполнить следующие команды для конфигурирования вашего локального гита.

Настройки принадлежности ваших коммитов:
```bash
$ git config --global user.name "Your Name Surname"
$ git config --global user.email "your@e.mail"
```

Следующая команда необходима для того, чтобы пушить на гитлаб только текущую ветку.
Необходимо **ВСЕМ**.
```bash
$ git config --global push.default current
```

Некоторые цветовые настройки. Будет красиво:
```bash
$ git config --global color.branch true
$ git config --global color.interactive true
$ git config --global color.diff true
$ git config --global color.grep true
$ git config --global color.status true
```

## Выполнение
Каждое задание находится в отдельной ветке с именем **hw-_num_**, где _num_ - номер задания.
Описание задания находится в каждой ветке в *README.md*: его нужно выполнить, закоммитить и отправить на сервер.
На каждый пуш GitLab запустит CI, который: проверит стиль кода, соберет проект, прогонит различные тесты.

**Note: все линтеры, сборщики и тесты находятся в той же ветке, что и задание. Их можно и нужно запустить локально,
 перед отправкой на сервер**

Перед тем как начать делать задание, вам необходимо создать новую ветку с любым удобным именем
(например, **making-hw-_num_**, _num_ - номер задания) от ветки **hw-_num_**:
```bash
$ git checkout hw-1
$ pit push
$ git checkout -b making-hw-1
```

После этого нужно делать домашнее задание в этой ветке.

## Защита
После успешного прогона CI вы можете отправить задание на проверку.
Для этого нужно сделать **Merge Request** вашей ветки ( **making-hw-_num_** в примере)
в ветку **hw-_num_** (_num_ - номер задания) и добавить в **Assignee** любого из преподавателей.

**Note: Merge Request необходимо делать в свой форк!**

Если после проведения code-review остались замечания или сомнения по коду, то оно может быть отправлено на доработку
 (на усмотрение преподавателя).

**Note: выполненным и защищенным будет считаться задание со смёрдженным преподавателем MR'ом и закрытыми дискуссиями.**
Если вы сами закроете дискуссии или сами вмержите код, оценка на портал не попадет. Ее проставляет все равно препод.

Лучше отвечайте на комментарии в MR. Чтобы преподаватель видел, что вы онлайн. Вас много, нам сложно мониторить когда чего вы
поменяли в коде.

### Merge Request:
![alt text][merge_request_1]
![alt text][merge_request_2]
