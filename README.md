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

## Выполнение
Каждое задание находится в отдельной ветке с именем **hw-_num_**, где _num_ - номер задания.
Описание задания находится в каждой ветке в *README.md*: его нужно выполнить, закоммитить и отправить на сервер.
На каждый пуш GitLab запустит CI, который: проверит стиль кода, соберет проект, прогонит различные тесты.

**Note: все линтеры, сборщики и тесты находятся в той же ветке, что и задание. Их можно и нужно запустить локально,
 перед отправкой на сервер**

## Защита
После успешного прогона CI вы можете отправить задание на проверку.
Для этого нужно сделать **Merge Request** в специальную ветку **hw-_num_-done** (_num_ - номер задания) и добавить
 в **Assignee** любого из преподавателей.

**Note: Merge Request необходимо делать в свой форк!**

Если после проведения code-review остались замечания или сомнения по коду, то оно может быть отправлено на доработку
 (на усмотрение преподавателя).

**Note: выполненным и защищенным будет считаться задание со смёрдженным преподавателем MR'ом**

### Merge Request:
![alt text][merge_request_1]
![alt text][merge_request_2]