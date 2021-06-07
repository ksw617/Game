using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class TimeManager : MonoBehaviour
{
    public Text text;
    public Transform sun;

    [Header("Rate")]
    [Range(1, 100)]
    public int speed = 1;

    [Header("Time")]
    public int gameDayToSecond = 1;

    private int dayToMonth = 31;

    [SerializeField] private float time;
    private float dayRate;

    int year;
    int month;
    int day;

    const int dayToSecond = 86400;

    private void Awake()
    {
        dayRate = (float)dayToSecond / gameDayToSecond;
    }

    private void Start()
    {
        year = 0;
        month = 1;
        day = 1;
    }

    private void FixedUpdate()
    {
        CalculateDate();
     
       // sun.Rotate(Vector3.right * (7.2f / dayToSecond) * speed);
    }

    void CalculateDate()
    {
        time += Time.fixedDeltaTime * speed * dayRate;
        int second = (int)time % 60;
        int min = ((int)time / 60 % 60);
        int hour = ((int)time / 3600 % 24);

        if(time >= 3600 * 24) 
        {
            Debug.Log("test");
            time = 0f;
           
            day++;
            if (day == dayToMonth) // dayToMonth : 31 30 29 28
            {
                day = 1;
                month++;
                dayToMonth = GetDayToMonth();
                if (month == 13)
                {
                    month = 0;
                    year++;
                }
              
            }
        }

       

       

        text.text = $"{year}³â{month}¿ù{day}ÀÏ \n {hour}:{min}:{second}";
    }

    //                      0       1
    int GetDayToMonth()
    {
        //   
        if (month <= 7)
        {

            if (month == 2)
            {
                return GetLeftYear();
            }


            if (month % 2 == 0)
            {
                return 30;
            }
            else
            {
                return 31;
            }
        }
        else
        {
            if (month % 2 == 0)
            {
                return 31;
            }
            else
            {
                return 30;
            }
        }
    }

    int GetLeftYear()
    {
        if (year % 4 == 0)
        {
            if (year % 400 != 0 && year % 100 == 0)
            {
                return 28;
            }
            else
            {
                return 29;
            }
        }

        return 28;

    }

}
