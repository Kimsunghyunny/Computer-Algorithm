# 컴퓨터 알고리즘 - 유전자 정보 복구 알고리즘
컴퓨터 알고리즘 프로젝트 - 전장 유전자 정보 복구 알고리즘 - KMP + Alpha Skip 
<br>

## 프로젝트 목표
인간의 유전자 DNA는 상상할 수 없을 정도로 많은 수를 가지고 있다. 이러한 유전자는 각 인간의 특징과 건강사항 등을 결정짓는데 큰 역할을 한다. 따라서 인간의 DNA를 분석하는 것이 대두되고 있다. 30억의 수 많은 DNA중에서, 모든 인간이 가지고 있는 99.9%가 아닌 다른 구조를 가진 0.1%의 DNA를 찾아내어 인간의 특징과 건강을 예측할 수 있게 된다. 따라서 그러한 다른 DNA를 찾아내기 위해서 이번 프로젝트에서는 A/C/G/T로 구성되어 있는 랜덤한 n개의 문자열을 생상하여 gemone을 만든 뒤, 특정 l길이의 short read를 m개 잘라내서 생성한다. 또한 genome과 0.1%가 다른 새로운 reference를 만들어, short read와 reference를 이용하여 원래의 genome을 복구하는 것이 목표이다.
<br>
## Bench Mark Algorithm
벤치마크로 사용한 알고리즘은 KMP 알고리즘이다. KMP알고리즘은 Θ(n)의 matching time complexity를 가진다. 하지만 전수조사를 하는 Trivial Mapping 방법은 Θ(nm)의 time complexity를 가진다. 따라서 KMP 알고리즘은 전수조사의 방법에 비해서 더 좋은 시간 효율을 가지고 있다. 하지만 KMP는 mismatch를 고려해서 string matching을 실행할 때, 한번 지나간 string에 대해서는 다시 탐색하지 않아서 정확도가 떨어지는 경우가 있다. 가령 CGTTA 인 pattern과 ACCTTACG의 string을 matching하고 있다면, <그림1>과 같이 mismatch가 3이 되는 순간 KMP검색 방법에 의하여 틀린 글자인 C와 같은 patter의 첫번째 글자 C로 돌아가서 <그림2>와 같이 KMP 탐색을 다시 시작한다. <그림2>에서도 mismatch가 3이되어 pattern을 찾아내지 못했다. 하지만 KMP가 건너뛴 부분을 보면 CGTTA와 mismatch가 1값을 갖는
string을 <그림3>과 같이 찾을 수 있다. 이러한 경우 때문에 KMP의 정확도가 떨어지게 된다. 정리하자면, KMP는 최악의 경우에 대한 좋은 효율성을 가지고 있기 때문에 Trivial에 비해서 정확도는 떨어지지만 시간적으로 좋은 알고리즘이라는 생각을 하여서 선택하였다.

![image](https://user-images.githubusercontent.com/22141977/117793579-4f8e7f80-b287-11eb-9cad-e8d523bfc1d4.png)
<그림1>

![image](https://user-images.githubusercontent.com/22141977/117793589-51f0d980-b287-11eb-8ff9-70f502930854.png)
<그림2>

![image](https://user-images.githubusercontent.com/22141977/117793618-59b07e00-b287-11eb-97db-eb030213c00f.png)
<그림3>

<br>

## My Algorithm
앞서 BenchMark Algorithm 에서 말한 것과 같이 KMP 는 시간적인 면에서 효 율 적이지만 정확도가 떨어진다는 단점을 가지고 있 다 하지만 우리가 목표로 하는 genome 복구는 정확도와 복구하는 정도가 굉장히 중요하기 때문에 KMP 의 알고리즘을 이용하여서 genome 복구를 하는 것은 좋지 못하다고 판단되었다 따라서 K MP 의 방법과 같이 시간을 빠르게 이용하여 탐색할 수 있는 방법을 토대 로 K MP 검색 시에 놓칠 수 있는 부분에 대하여 Alpha skip searching Algorithm 을 이용하여 탐색하는 알고리즘을 생각하게 되었다. Alpha searchin g Algorithm 은 p attern 에 대하여 3 글자씩 잘라내어 각 3 글자로 잘라낸 pattern 의위치를 Tries data structure 을 이용하여 저장한다 이후에 특정 위치마다 Tries 를 검색하여 p attern이 존재하는지 여부를 확인하고 존재할 시에 해당 위치에서의 s tring 탐색을 시도한다 Alpha searching 을 실행하는 i의위치는 다음과 같은 수식을 이용하여 구하게 하였다.
<br><font size = "20"><b>𝑙𝑜𝑐𝑎𝑡𝑖𝑜𝑛𝑛=𝑚+6(𝑛+1)−9 (n≥0)</font></b><br>
다음의 수식에 존재하는 글자 뒤로 2글자를 포함한 3글자를 pattern의 위치를 저장한 자료구조에서 찾아내서 일치하는 pattern 이전과 이후의 글자를 string matching 하여 KMP가 놓칠수있는 pattern matching 을 찾아내도록 한다.

<br>

## Result
<br>

![image](https://user-images.githubusercontent.com/22141977/117794140-ebb88680-b287-11eb-8587-91855418bbed.png)

<kmp_result.txt>


![image](https://user-images.githubusercontent.com/22141977/117794173-f2df9480-b287-11eb-990e-c0f57e02cf3c.png)
<N = 1,000, M =50~200 , L =20~30, D =2>

