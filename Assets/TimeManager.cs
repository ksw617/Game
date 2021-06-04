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
    public int dayToSecond;

    private int dayToMonth = 31;

    [SerializeField] private float time;
    private float dayRate;

    private void Awake()
    {
        dayRate = 86400f / dayToSecond;
    }

    private void FixedUpdate()
    {
        time += Time.fixedDeltaTime * speed * dayRate;
        int second = (int)time % 60;
        int min = ((int)time / 60 % 60);
        int hour = ((int)time / 3600 % 24);
        int year = (int)time / 31536000;
  
        int month = (int)time / (86400 * dayToMonth) % 12;
        int day = (int)time / 86400 % dayToMonth;
tu
        dayToMonth = GetDayToMonth(year, month);

        sun.Rotate(Vector3.right * (7.2f / dayToSecond) * speed);
    }

    int GetDayToMonth(int year, int month)
    {
        if (month <= 7)
        {

            if (month == 2)
            {
                return GetLeftYear(year);
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

    int GetLeftYear(int year)
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
